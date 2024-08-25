/* Intialize Temperature Sensors */
int InitTempSensors(void) {
  numTS = MCPSensors + DHTSensors + DS18B20Sensors;
  DisplayPrint(String(MCPSensors) + " MCP9808, " + String(DHTSensors) + " DHT Sensors and " + String(DS18B20Sensors) + " DS18B20 Sensors Installed"); 
  if (numTS > 3) {
    DisplayPrint("Too many Temperature Sensors Defined");
  }
  if (MCPSensors > 0) Wire.begin(SDApin, SCLpin);
  if(DHTSensors > 0) {
    dht1.begin();
    if(DHTSensors > 1) dht2.begin();
    if(DHTSensors > 2) dht3.begin();
  }
  if (DS18B20Sensors > 0) DS18B20Start();
  ReadTempSensors();  
  int goodTS = 0;
  for (int i = 1; i <= numTS; i++) {
    if (Temperature[i] > 0) goodTS++;
  }  
  if (goodTS > 0) {return goodTS;} else {return 0; }
}  

/* Get Temperature  */ 
void GetTemperature(void) {
  if(numTS != 0) {
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
}  
/* Check if Freezer needs to be turned On or Off */
void CheckFreezer( void ) {
static long FreezerStartTime = 0;
static long FreezerOnTime = 0;
  
  if (TempAvailFlag) {                                             /* Is Temperature Available? */ 
    if (Temperature[UsingSensor] > Tempgoal)  {                    /* Is Temperature too high? */
      if (FreezerState == 0) {
        FreezerControl(HIGH);                                        /* Turn on Freezer */
        FreezerStartTime = millis();
      }
    }
    if (Temperature[UsingSensor] < Tempgoal - 0.5) {                         /* Is Temperature too low? */
      if (FreezerState == 1) {
        FreezerControl(LOW);                                                  /* Turn off Freezer */ 
        unsigned long cTime = millis();
        FreezerOnTime = FreezerOnTime + (cTime - FreezerStartTime);
        FreezerOnPercent = ((float)FreezerOnTime / cTime) * 100.0;
      }
    }
    if (Temperature[UsingSensor] < 32) {                                    /* Is Temperature too low? */
      SwitchtoSecondary();
      ProcAlarm(Alarm10, "Freezer too cold, Switch to Secondary");
    }  
    if (Temperature[UsingSensor] > 40) {                                    /* Is Temperature too low? */
      SwitchtoSecondary();
      ProcAlarm(Alarm11, "Freezer too hot, Switch to Secondary");
    }  
  }
  else {                                                  /* Temperature is not Available */
    FreezerControl(LOW);                                  /* Turn off Freezer */ 
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
  if (TempCount <= AvgCount) {                                             /* retune running average */
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
      MinPressReading = MaxPressReading;
}
float ReadMCPSensor(MCP9808 &sensor, int sn) {
  static int count[] = {0, 0, 0, 0}; static int errcnt[] = {0, 0, 0, 0};
  static float previous[3]; float temp = -1;

  if (sensor.getManufacturerID() != 0x0054) {
    errcnt[sn]++;
    Serial.println("MPC9808 ManufacturerID Error " + String(sn) + " Error Cnt " + String(errcnt[sn]));
    if (errcnt[sn] > 6) {
      ProcAlarm(Alarm6, "I2C Bus Error #" + String(sn));
      return -1;
    }  
  }
  if (sensor.getDeviceID() != 0x04) {
    Serial.println("MPC9808 Device ID Error " + String(sn));
    errcnt[sn]++;
    if (errcnt[sn] > 6) {
      ProcAlarm(Alarm6,"Device ID Error, Sensor #" + String(sn));
      return -2;
    }  
  }
  if (errcnt[sn] > 0) return -4;
  temp = CtoF(sensor.getTemperature());
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
  errcnt[sn] = 0;
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
  float t = 0; float h = 0; String err = ""; static int ec[] = {0, 0, 0, 0};
  //Serial.println("Reading Sensor " + String(sn));
  err = sensor.getDataRetry(h, t);
  if (err != "OK") {
    ec[sn]++;
    if (ec[sn] > 3) {
        ProcAlarm(Alarm8, "DHT #" + String(sn) + " Read Err, " + err); return -1;
    }
  }
  t = CtoF(t);  
  ec[sn] = 0;
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
  if (DS18B20Sensors > 0) {
    sensors.requestTemperatures();  //issue a global temperature request to all devices on the bus
    Temperature[SN] = ReadDS18B20(DS18B20Temp1, SN);
    if (DS18B20Sensors > 1) {
      SN++;
      Temperature[SN] = ReadDS18B20(DS18B20Temp2, SN);
    }
    if (DS18B20Sensors > 2) {
      SN++;
      Temperature[SN] = ReadDS18B20(DS18B20Temp3, SN);
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
      FreezerControl(LOW);
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
      FreezerControl(LOW);
      return false;
    }
    if(TempGood[1] && TempGood[2] && TempGood[3]) {                                /* All temperature Sensors are good */
      if(abs(Temperature[1] - Temperature[2]) < MaxDeltaTemp  && abs(Temperature[1] - Temperature[3] < MaxDeltaTemp)) {
        UsingSensor = 1; return true;
      }
      else {                                                                        /* Not within tolerance */
        float delta12 = abs(Temperature[1] - Temperature[2]);
        float delta13 = abs(Temperature[1] - Temperature[3]);
        float delta23 = abs(Temperature[2] - Temperature[3]);
        if (delta12 < delta13 && delta12 < delta23) {                              /* delta12 is smallest */
          ProcAlarm(Alarm5, "Sensors 3 is not reasonable, " + String(Temperature[3]));
          UsingSensor = 1; return true;
        }  
        if (delta13 < delta12 && delta13 < delta23) {                              /* delta13 is smallest */
          ProcAlarm(Alarm5, "Sensors 2 is not reasonable, " + String(Temperature[2]));
          UsingSensor = 3; return true;
        }
        if (delta23 < delta12 && delta23 < delta13) {                              /* delta23 is smallest */  
          ProcAlarm(Alarm5, "Sensors 1 is not reasonable, " + String(Temperature[1]));
          UsingSensor = 2; return true;
        }  
        ProcAlarm(Alarm4, "No match, #1 " + String(Temperature[1],1) + " #2 " + String(Temperature[2],1) + " #3 " + String(Temperature[3],1));
      }  
    }
    for (int i = 1; i <= numTS; i++){                                              /* at least one Sensor is good */
      if (TempGood[i] = true) {
        UsingSensor = i; return true;
      }
    }    
  }
}

float ReadDS18B20(DeviceAddress deviceAddress, int devnum)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
  {
    ProcAlarm(Alarm12, "DS18B20 sensor # " + String(devnum) + " read error");
    //Serial.println("Error: Could not read temperature data");
    return -1;
  }
  tempC = CtoF(tempC);
  //if (tempC > 90 || tempC < 20) return -2;
  return tempC;
}

void DS18B20Start(void) {
  if (DS18B20Sensors > 0) {
    sensors.begin();
    int numDS18B20 = sensors.getDeviceCount();
    if (DS18B20Sensors != numDS18B20) {
      DisplayPrint("Error, found " + String(numDS18B20) + " expected " + String(DS18B20Sensors) + " DS18B20 Sensors");
    }
    if (!sensors.getAddress(DS18B20Temp1, 0)) { 
      DisplayPrint("Unable to find DS18B20 Device 1");
    }  
    else {
      sensors.setResolution(DS18B20Temp1, TEMPERATURE_PRECISION);
    }
    if (DS18B20Sensors > 1) {
      if (!sensors.getAddress(DS18B20Temp2, 1)) {
        DisplayPrint("Unable to find DS18B20 Device 2");
      }
      else {
        sensors.setResolution(DS18B20Temp1, TEMPERATURE_PRECISION);
      }
    }
    if (DS18B20Sensors > 1) {
      if (!sensors.getAddress(DS18B20Temp3, 2)) {
        DisplayPrint("Unable to find DS18B20 Device 3");
      }
      else {
        sensors.setResolution(DS18B20Temp1, TEMPERATURE_PRECISION);
      }  
    }  
  }
}

void FreezerControl (int state) {
  
  if (digitalRead(PrimarySecondaryPin) == Primary) {
    if (digitalRead(PrimaryOnOffPin) != state) {
      PowerDownScales();    
      digitalWrite(PrimaryOnOffPin, state);
      FreezerState = state;
      PowerUpScales();
    }  
  }
  else {
    if (digitalRead(SecondaryOnOffPin) != state) {
      PowerDownScales();
      digitalWrite(SecondaryOnOffPin, state);
      FreezerState = state;
      PowerUpScales();
    }
    
  }  
  
}

void SwitchtoSecondary(void) {
  digitalWrite(PrimaryOnOffPin, LOW);                                   /* Turn off Freezer */ 
  delay(100);
  digitalWrite(PrimarySecondaryPin, Secondary);                    
}
