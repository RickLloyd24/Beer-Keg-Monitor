#define MaxReading 4.5*4095/3.2
#define MinReading 0.5*4095/3.2   //640
#define MaxVoltage 4.5
#define MinVoltage 0.5
#define MaxPressure 2500
#define LowPressure 250
#define MinPressure 100
#define MinPoints 9
#define CO2OnDuration 5000        //Valve on for 5 seconds 

void ReadCO2Pressure(void) {
  CO2Pressure = analogRead(CO2PressurePin);
  CO2Pressure = (CO2Pressure - MinReading) * MaxPressure / (MaxReading - MinReading);   //convert to psi
  //CO2Pressure = 500 + random(0, 100);
  CO2PressureBuf[CO2BufPtr] = CO2Pressure; 
  CO2BufPtr++; if(CO2BufPtr >= CO2BufLen) CO2BufPtr = CO2BufLen - 1;
}
void CheckCO2 (void) {
  CO2OnTime = curtime + 60*60*1000;                                        //Check again in 1 hour
  if (CO2Rate != 60 && CO2Reset == 0 && CO2Rate != 99) {
    CO2Reset = CO2OnTime*2;                                                //Reset Rate to 60 in 2 Hours  
  }          
  if (curtime > CO2Reset) {CO2Reset = 0; CO2Rate = 60;}                    //Reset Rate back to 60 minutes
  if (CO2Rate == 0) {                                                      //Valve is on Continuously
    if (digitalRead(CO2ValvePin) == LOW) {
      PowerDownScales();
      digitalWrite(CO2ValvePin, HIGH);                                     //Turn On CO2 Valve
      PowerUpScales();
    }  
  }
  else if (CO2Rate == 99) {                                                //Valve is off Continuously
    digitalWrite(CO2ValvePin, LOW);                                        //Turn Off CO2 Valve
  }
  else if (digitalRead(CO2ValvePin) == HIGH) {                             //Valve is ON
    ReadCO2Pressure();                                                     //Read Pressure while on
    PowerDownScales();
    digitalWrite(CO2ValvePin, LOW);                                        //Turn Off CO2 Valve
    PowerUpScales();
    CO2PressureBuf[0] = CO2Pressure;                                       //Save the first point as max pressure
    MaxPressReading = CO2Pressure;
    MinPressReading = CO2Pressure;
    CO2PressureChg = 0;
    CO2OnTime = curtime + CO2Rate*60*1000;                                 //Set Next On time by rate
    if (CO2BufPtr > MinPoints) {
      prevOffset = offset; prevSlope = slope;
      Serial.println("prevOffset = " + String(offset) + ", prevSlope = " + String(slope));
      LinearRegression();                                                    //Calculate Straight Line Fit
      MaxPressReading = offset + 1 * slope;                                  //Beginning of Line
      MinPressReading = offset + CO2BufPtr * slope;                          //End of Line  
      Serial.println("MaxPressReading = " + String(MaxPressReading) + ", MinPressReading = " + String(MinPressReading));
      CO2PressureChg = MinPressReading - MaxPressReading;                    //Calculate Change in Pressure based upon slope of line
    }  
    if (CO2BufPtr > 99) {  
      if (MaxPressReading < MinPressure) {
        ProcAlarm(Alarm14, "CO2 Tank Pressure is Low, " + String(MaxPressReading));
      }  
      if (MinPressReading < 100) {
        ProcAlarm(Alarm15, "CO2 Reading below " + String(MinPressReading) + " PSI, Leak");  
        CO2Rate = 99;
      }
      if (abs(CO2PressureChg) > 100) {
        ProcAlarm(Alarm13, "Large CO2 PSI drop " + String(CO2PressureChg));  
      }
    }
    CO2BufPtr = 1;                                                          //Set Buffer Point for second point  
  }
  else {                                                                   //Valve is OFF
    PowerDownScales();
    digitalWrite(CO2ValvePin, HIGH);                                       //Turn On CO2 Valve
    CO2OnTime = CO2OnDuration + curtime;                                   //Turn On CO2 for short duration
  }
}

/* Calculate Linear Regression 
Calculates the coefficients offset and slope for equation y = offset + slope*x   */
void LinearRegression (void) {
  float SumX = 0; float SumY = 0; float SumXY = 0; float SumX2 = 0; float SumY2 = 0;
  offset = 0; slope = 1;
  if (CO2BufPtr < 3) return;
  for (int i = 0; i < CO2BufPtr; i++) {
    int X = i + 1;
    int Y = CO2PressureBuf[i];
    SumX = SumX + X;
    SumY = SumY + Y;
    SumXY = SumXY + (X*Y);
    SumX2 = SumX2 + (X*X);
    SumY2 = SumY2 + (Y*Y);
  }
  offset = (SumY*SumX2 - SumX*SumXY)/(CO2BufPtr*SumX2 - SumX*SumX);      // [(ΣY)(ΣX2) – (ΣX)(ΣXY)]  /  [n(ΣX2) – (ΣX)2]
  slope = (CO2BufPtr*SumXY - SumX*SumY)/(CO2BufPtr*SumX2 - SumX*SumX);  //[n(ΣXY) – (ΣX)(ΣY)]  /  [n(ΣX2) – (ΣX)2]
}

void CO2OnlyLoop( void ) {
  DisplayPrint("CO2 Only Display");
  ScreenSaverStartTime = curtime + ScreenSaverStart;
  CO2OnTime = curtime + TENSECONDMILS*3 + 600;
  while (1) {
    curtime = millis();
/* Check if a key has been pressed */
    auto keyboard = PS2Controller.keyboard();
    if (keyboard->virtualKeyAvailable()) {
      VirtualKeyItem item;
      if (keyboard->getNextVirtualKey(&item)) {
        char key = item.ASCII;
        if(item.down == 0) {                                                 /* Key Released */
          //Serial.print(key); Serial.println(" key Pressed");
          ProcessKeyCO2Plot(key);
        }
      }
    }      
  /* CO2 Control Tasks */  
    if (curtime > CO2OnTime) CheckCO2();
    if (curtime > Tensecond) {
      Tensecond = Tensecond + TENSECONDMILS;
      ReadCO2Pressure();
    }  
/* 1 Minute Tasks */  
    if (curtime > OneMinuteTT) {         
      OneMinuteTT = OneMinuteTT + MINUTEMILS;
      if (curtime > ScreenSaverStartTime) {
        if (ScreenSaverOn == false) {
          cv.clear();
          ScreenSaverOn = true;
          Serial.println("Screen Saver is On");
        }  
      }
      if (ScreenSaverOn == false) DisplayPlot();
    }
  }
}
