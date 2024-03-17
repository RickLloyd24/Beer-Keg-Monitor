
/* Intialize Temperature Sensors */
int InitTempSensors(void) {
  numTS = MCPSensors + DHTSensors;
  if (MCPSensors > 0) Wire.begin(SDApin, SCLpin);
  DisplayPrint(String(MCPSensors) + " MCP9808 and " + String(DHTSensors) + " DHT Sensors Installed"); 
  if (numTS > 3) {
    DisplayPrint("Too many Temperature Sensors Defined");
    return 0;
  }
  if(DHTSensors > 0) {
    dht1.begin();
    if(DHTSensors > 1) dht2.begin();
    if(DHTSensors > 2) dht3.begin();
  }
  ReadTempSensors();  
  int goodTS = 0;
  for (int i = 1; i <= numTS; i++) {
    if (Temperature[i] > 0) goodTS++;
  }  
  if (goodTS > 0) {return goodTS;} else {return 0; }
}  

/* Get Temperature  */ 
void GetTemperature(void) {
  ReadTempSensors();
  for (int i = 1; i<= numTS; i++) {
    if (Temperature[i] > 0) Temperature[i] = Temperature[i] + TempBias[i];    
  }
  String s = "Temp values ";
  int i;
  for (i = 1; i < numTS; i++){
    s = s + String(Temperature[i],2) + ", ";
  }
  s = s + String(Temperature[i],2);
  //Serial.println(s);
  if (SelectTemperatureSensor()) {
    Temperature[0] = MovingAverage(Temperature[UsingSensor]);
    if (Temperature[UsingSensor] > HighTemp) HighTemp = Temperature[UsingSensor];                             /* New High Temperature? */
    if (Temperature[UsingSensor] < LowTemp)  LowTemp = Temperature[UsingSensor];                               /* New Low Temperature? */
  }  
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
void ClearTempStatistics(void) {
  const int MaxTempVariance = 10;
  
      MaxTemp = Tempgoal + MaxTempVariance;   
      MinTemp = Tempgoal - MaxTempVariance;
      HighTemp = MinTemp;                    
      LowTemp = MaxTemp;  
      MovingAverage(-1);
      Temperature[0] = 0;
}
float ReadMCPSensor(MCP9808 &sensor, int sn) {
  static int count[] = {0, 0, 0};
  static float previous[3]; float temp = -1;

  if (sensor.getManufacturerID() != 0x0054) {
    ProcAlarm(Alarm6, "I2C Bus Error #" + String(sn));
    return -1;
  }
  if (sensor.getDeviceID() != 0x04) {
    ProcAlarm(Alarm6,"Device ID Error, Sensor #" + String(sn));
    return -2;
  }
  temp = CtoF(sensor.getTemperature());
  sn--;
  if (temp == previous[sn]) {
    count[sn]++;
    if (count[sn] > 2*60*60) {      // 2 hours same temperature
      ProcAlarm(Alarm6,"Temperature Stuck, Sensor #" + String(sn));
      return -3;
    }
  }
  else {
    previous[sn] = temp;
    count[sn] = 0; 
  }
  return temp;
}
float CtoF(float C) {
  return ((C*9.0/5.0) + 32.0);
}
float FtoC(float F) {
  return ((F - 32.0) * 5.0/9.0);
}
/* Read Temperature from Sensor */
float ReadDHT(DHTSimple &sensor, int sn) {
  float t = 0; float h = 0; String err = ""; static boolean ec[] = {false, false, false, false};
  //Serial.println("Reading Sensor " + String(sn));
  err = sensor.getDataRetry(h, t);
  if (err != "OK") {
    if (DHTPower) {
      if (ec[sn]) {
        ProcAlarm(Alarm8, "DHT #" + String(sn) + " Read Err, " + err); return -1;
      }  
      else {  
        ResetDHT(sn - MCPSensors);
        Serial.println("Error " + err + " Temperature " + String(t));
        Serial.println("Reset Sensor " + String(sn));
        ec[sn] = true;
        return Temperature[0];
      }
    }
    else {
      ProcAlarm(Alarm8, "DHT #" + String(sn) + " Read Err, " + err); return -1;
    }
  }
  t = CtoF(t);  
  ec[sn] = false;
  return t;  
}
void ReadTempSensors(void) {
  int SN = 1;
  if (MCPSensors > 0) {
    Temperature[SN] = ReadMCPSensor(Sensor1, SN);
    SN++;
    if (MCPSensors > 1) {
      Temperature[SN] = ReadMCPSensor(Sensor2, SN);
      SN++;
    }
    if (MCPSensors > 2) {
      Temperature[SN] = ReadMCPSensor(Sensor3, SN);
    }
  }
  if (DHTSensors > 0) {
    Temperature[SN] = ReadDHT(dht1, SN);
    SN++;
    if (DHTSensors > 1) {
      Temperature[SN] = ReadDHT(dht2, SN);
      SN++;
    }
    if (DHTSensors > 2) {
      Temperature[SN] = ReadDHT(dht3, SN);
    }
  }
}
/* Decide which Temperature Sensor to Use */
boolean SelectTemperatureSensor(void) {
  boolean TempGood[4];
  if (numTS == 1) {
    if (Temperature[1] > 0) {UsingSensor = 1; return true;} else {UsingSensor = 0; return false;}
  }
  for (int i = 1; i <= numTS; i++){
   TempGood[i] = (Temperature[i] > 0) ? true : false;
  }
  if (numTS == 2) {
    if (TempGood[1] == false && TempGood[2] == false) {                                /* Are both bad? */
      ProcAlarm(Alarm7, "Both Temp Sensors Bad " + String(Temperature[1], 1) + ", " + String(Temperature[2], 1)); 
      digitalWrite(FrigControlPin, LOW);
      return false;
    }
    if (TempGood[1] == true && TempGood[2] == true) {                                  /* Are both good? */
      if (abs(Temperature[1]-Temperature[2]) < MaxDeltaTemp) {                     
        UsingSensor = 1;
        return true;
      }
      ProcAlarm(Alarm4, "Sensors don't match, 1-2 " + String(Temperature[1]-Temperature[2]));
      int Temp1GoalDelta = abs(Temperature[1] - Tempgoal);
      int Temp2GoalDelta = abs(Temperature[2] - Tempgoal);
      if (Temp1GoalDelta > Temp2GoalDelta) {                                     /* Pick Sensor closest to goal */
        UsingSensor = 2; return true;
      }
      else {
        UsingSensor = 1; return true;
      }
    }  
  }
  if (numTS == 3) {  
    if(TempGood[1] == false && TempGood[2] == false && TempGood[3] == false) {       /* All temperature Sensors Bad */
      ProcAlarm(Alarm7, "All Temp Sensors Bad " + String(Temperature[1], 1) + ", " 
          + String(Temperature[2], 1) + ", " + String(Temperature[3], 1));    
      digitalWrite(FrigControlPin, LOW);
      return false;
    }
    if(TempGood[1] && TempGood[2] && TempGood[3]) {                                /* All temperature Sensors are good */
      if((Temperature[1] - Temperature[2]) < MaxDeltaTemp  && (Temperature[1] - Temperature[3] < MaxDeltaTemp)) {
        UsingSensor = 1; return true;
      }
      else {                                                                        /* Not within tolerance */
        float delta12 = abs(Temperature[1] - Temperature[2]);
        float delta13 = abs(Temperature[1] - Temperature[3]);
        float delta23 = abs(Temperature[2] - Temperature[3]);
        ProcAlarm(Alarm4, "No match, #1 " + String(Temperature[1],1) + " #2 " + String(Temperature[2],1) + " #3 " + String(Temperature[3],1));
        if (delta12 < delta13 && delta12 < delta23) {                              /* delta12 is smallest */
          ProcAlarm(Alarm5, "Sensors 3 is not reasonable, " + String(Temperature[3]));
          UsingSensor = 1; return true;
        }  
        if (delta13 < delta12 && delta13 < delta23) {                              /* delta13 is smallest */
          ProcAlarm(Alarm5, "Sensors 2 is not reasonable, " + String(Temperature[2]));
          UsingSensor = 1; return true;
        }
        if (delta23 < delta12 && delta23 < delta13) {                              /* delta23 is smallest */  
          ProcAlarm(Alarm5, "Sensors 1 is not reasonable, " + String(Temperature[1]));
          UsingSensor = 2; return true;
        }  
      }  
    }
    for (int i = 1; i <= numTS; i++){                                              /* at least one Sensor is good */
      if (TempGood[i] = true) {
        UsingSensor = i; return true;
      }
    }    
  }
}
void ResetDHT(int dhtnum) {
  if (dhtnum == 1) PowerDown(DHT1Power);
  if (dhtnum == 2) PowerDown(DHT2Power);
  if (dhtnum == 3) PowerDown(DHT3Power);
}
void PowerDown(int PinNum) {
  digitalWrite(PinNum, LOW);
  delay (300);
  digitalWrite(PinNum, HIGH);
  delay (1000);
}
