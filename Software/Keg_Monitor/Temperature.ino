/* Get Temperature  */ 
void GetTemperature(void) {
  Temperature[1] = GetSensorTemp(1, DHTPIN1);
  Temperature[2] = GetSensorTemp(2, DHTPIN2);
  if (Temperature[1] > 0) Temperature[1] = Temperature[1] + TemperatureBias[1];                /* Add bias if temp is OK */
  if (Temperature[2] > 0) Temperature[2] = Temperature[2] + TemperatureBias[2];
  Serial.println("Temp #1 " + String(Temperature[1],1) + ", #2 " + String(Temperature[2],1));
  CheckTemperature();
  Temperature[0] = MovingAverage(Temperature[UsingSensor]);
  if (Temperature[0] > HighTemp) HighTemp = Temperature[0];                             /* New High Temperature? */
  if (Temperature[0] < LowTemp)  LowTemp = Temperature[0];                               /* New Low Temperature? */
  CheckFreezer();
}  
/* Check if Freezer needs to be turned On or Off */
void CheckFreezer( void ) {
static long FreezerStartTime = 0;
static long FreezerOnTime = 0;
  
  if (TempAvailFlag) {                                             /* Is Temperature Available? */ 
    if (Temperature[UsingSensor] > Tempgoal)  {                    /* Is Temperature too high? */
      digitalWrite(FrigControlPin, HIGH);                          /* Turn on Freezer */
      if (FreezerState == 0) {
        FreezerStartTime = millis();
      }
      FreezerState = 1;                                              /* Set Freezer Stat to On */
    }
    if (Temperature[UsingSensor] < Tempgoal - 0.5) {                        /* Is Temperature too low? */
      digitalWrite(FrigControlPin, LOW);                                   /* Turn off Freezer */ 
      if (FreezerState == 1) {
        unsigned long cTime = millis();
        FreezerOnTime = FreezerOnTime + (cTime - FreezerStartTime);
        FreezerOnPercent = ((float)FreezerOnTime / cTime) * 100.0;
      }
      FreezerState = 0;                                   /* Set Freezer Stat to Off */
    }
  }
  else {                                                  /* Temperature is not Available */
    FreezerState = 0;                                     /* Set Freezer State to Off */
    digitalWrite(FrigControlPin, LOW);                    /* Turn off Freezer */ 
  }
}
/* Calculate Moving Average */
float MovingAverage(float t) {
  const int AvgCount = 180;                                                /* Average over 30 minutes */
  static int TempCount = 1;                                                   /* Count of samples */
  static float AvgTemp = 0;
  static float AvgSum = 0;
  
  if (t == -1) {                                                            /* Reset Moving Average? */  
    TempCount = 1;
    AvgTemp = AvgSum = 0; 
    return 0;
  }
  if (TempCount < AvgCount) {                                             /* retune running average */
    AvgSum = AvgSum + t;
    AvgTemp = AvgSum/TempCount;
    TempCount++;
  }
  else {                                                                  /* Enough samples moving avergae */
    AvgSum = AvgSum - AvgTemp + t;
    AvgTemp = AvgSum/AvgCount;  
  }
  return AvgTemp;
}
/* Calculate Moving Average */
float MovingAverage2(float t) {
  const int AvgCount = 30;                                                /* Average over 5 minutes */
  static int TempCount = 1;                                                   /* Count of samples */
  static float AvgTemp = 0;
  static float AvgSum = 0;
  
  if (t == -1) {                                                            /* Reset Moving Average? */  
    TempCount = 1;
    AvgTemp = AvgSum = 0; 
    return 0;
  }
  if (TempCount < AvgCount) {                                             /* retune running average */
    AvgSum = AvgSum + t;
    AvgTemp = AvgSum/TempCount;
    TempCount++;
  }
  else {                                                                  /* Enough samples moving avergae */
    AvgSum = AvgSum - AvgTemp + t;
    AvgTemp = AvgSum/AvgCount;  
  }
  return AvgTemp;
}

/* Decide which Temperature Sensor to Use */
void CheckTemperature(void) {
  boolean Temp1Good = true; boolean Temp2Good = true;
  
  TempAvailFlag = true; 
  if (Temperature[2] < 0) Temp2Good = false;
  if (Temperature[1] < 0) Temp1Good = false; 
  if (!Temp1Good && !Temp2Good) {                                            /* both sensors are bad */
    TempAvailFlag = false;
    ProcAlarm(Alarm7, "Both Temp Sensors Bad " + String(Temperature[1], 1) + ", " + String(Temperature[2], 1)); 
    return;
  }
  if (Temp1Good && Temp2Good == false) {                                      /* #1 good #2 is bad */
    UsingSensor = 1;
    return;
  }
  if (Temp1Good == false && Temp2Good) {                                      /* #1 bad #2 is good */
    UsingSensor = 2;
    return;
  }
  if (Temp1Good && Temp2Good) {                                               /* both sensors good */
    if (abs(Temperature[1]-Temperature[2]) < MaxTempDelta) {                           
      TemperatureBias[0] = MovingAverage2(Temperature[1]-Temperature[2]);
      UsingSensor = 1;
      return;
    }
/* Sensors dont match error checking  */
    ProcAlarm(Alarm4, "Temp Sensors don't match, " + String(Temperature[1],1) + ", " + String(Temperature[2],1));
    if (Temperature[1] > MaxTemp || Temperature[1] < MinTemp) {            
      Temp1Good = false;
      ProcAlarm(Alarm5, "Temp Sensor 1 Unreasonable " + String(Temperature[1], 1));
      ResetDHT(1);
    }  
    if (Temperature[2] > MaxTemp || Temperature[2] < MinTemp) {                        
      Temp2Good = false;
      ProcAlarm(Alarm6, "Temp Sensor 2 Unreasonable " + String(Temperature[2], 1));
      ResetDHT(2);
    }  
    if (Temp2Good == false && Temp1Good == false) {                                       /* Are Both sensors are bad */
      TempAvailFlag = false; 
      return;
    }
    if (Temp1Good == true) {                                                          /* Is Sesor 1 good? */
      if (UsingSensor != 1) { 
        MovingAverage(-1);
        UsingSensor = 1;
      }
    }  
    else {                                                                                  /* Sensor 2 is good */ 
      if (UsingSensor != 2) { 
        MovingAverage(-1);
        UsingSensor = 2;
      }
    }  
  }
}
/* Read Temperature from Sensor */
float GetSensorTemp(int sn, int pin) {
static int ErrorCnt[3];
float humidity = 0; float t;
  uint8_t error=read_dht(t, humidity, pin, DHT22, 0);                /* Get Temperature Sensor 1 */
  if(error){  /* Codes-0: OK 1: TOO_SOON 2: DRIVER 3: TIMEOUT 4: NACK 5: BAD_DATA 6: CHECKSUM 7: UNDERFLOW 8: OVERFLOW */
    Serial.print("Error Sensor #"); Serial.print(sn); Serial.println(ErrorCode(error)); 
    ErrorCnt[sn]++;
    if (ErrorCnt[sn] > 3) {
      ProcAlarm(Alarm8, "Error Temp Sensor #" + String(sn) + ErrorCode(error));
      t = -error;
    } 
    else {
    t = Temperature[sn];                                                            /* Return old temperature */
    ResetDHT(sn);
    }
  }
  else{
    ErrorCnt[sn] = 0;
    t = t * 9 / 5 + 32;                                                             /* Return new temperature */
  }
  return t;
}
void ResetDHT(int sn) {
  if(sn == 1) {
    digitalWrite(DHTPIN1Power, LOW);
    delay(100);
    digitalWrite(DHTPIN1Power, HIGH);
  }
  else {
    digitalWrite(DHTPIN2Power, LOW);
    delay(100);
    digitalWrite(DHTPIN2Power, HIGH);  
  }
}
void ClearTempStatistics(void) {
  const int MaxTempVariance = 10;
  
      MaxTemp = Tempgoal + MaxTempVariance;   
      MinTemp = Tempgoal - MaxTempVariance;
      HighTemp = MinTemp;                    
      LowTemp = MaxTemp;  
      MovingAverage(-1);
      MovingAverage2(-1);
      Temperature[0] = 0;
}
/* Codes-0: OK 1: TOO_SOON 2: DRIVER 3: TIMEOUT 4: NACK 5: BAD_DATA 6: CHECKSUM 7: UNDERFLOW 8: OVERFLOW */
String ErrorCode(int code) {
  switch (code) {
    case 1: 
      return(" Too soon");
      break;
    case 2: 
      return(" Driver");
      break;
    case 3: 
      return(" Time Out");
      break;
    case 4: 
      return(" Nack");
      break;
    case 5: 
      return(" Bad Data");
      break;
    case 6: 
      return(" Checksum");
      break;
    case 7: 
      return(" Underflow");
      break;
    case 8: 
      return(" Overflow");
      break;
    default: 
      return(" Unknown Error");
      break;
  }
}
