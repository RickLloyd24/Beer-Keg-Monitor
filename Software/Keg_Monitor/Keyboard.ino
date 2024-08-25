String s = "";

/* Process key strokes */
void ProcessKey(char key) {
  if (DisplayMode == Scales) {
    ProcessKeyScales(key);
  }
  else if (DisplayMode == Command) {
    ProcessKeyCommand(key);
  }
  else if (DisplayMode == Plot) {
    ProcessKeyCO2Plot(key);
  }
  else if (DisplayMode == Alarm) {
    if (key == Esc) {
      ClearDisplayUpdateStr();
      DisplayUpdate();
      DisplayMode = Normal;
    }
  }
  else if (DisplayMode == Normal) {
    //Serial.print("key Pressed "); Serial.println(key, DEC);
    if (ScreenSaverOn == true) ClearDisplayUpdateStr();
    ScreenSaverOn = false;
    ScreenSaverStartTime = curtime + ScreenSaverStart;
    int tapn = key - '0';
    if (key == Esc) {
      DisplayMode = Command;
      InputError = "";
      InputResults = "";
      s = "";
      DisplayCommands();
    }
    else if (tapn > 0 && tapn <= numscales) {
      CurrentTap = tapn;
      DisplayUpdate();
    }
    else if (key == 'c') {
      ClearTempStatistics();
      DisplayUpdate();
    }  
    else if (key == 'a') {
      DisplayAlarms();
      DisplayMode = Alarm;
    }  
    else if (key == 's') {
      DisplayScaleCommands();
      DisplayMode = Scales;
    }  
    else if (key == 'p') {
      DisplayMode = Plot;
      DisplayPlot();
    }  
  }
}
     
void ProcessKeyCommand(char key) {
  static String s = "";
   
  //Serial.print("s String "); Serial.println(s);
  if (key == CarriageReturn) {
    ProcessCommand(s);
    s = "";
    if (DisplayMode == Command) DisplayCommands();
  }  
  else if (key == Backspace){
      s = s.substring(0, s.length() - 1);
      DisplayKeys(s);
  }
  else if (isPrintable(key)) { 
      s = s + String(key);
      DisplayKeys(s);
  }
  else if (key == Esc) {                                        /* Exit Command Mode */
      ClearDisplayUpdateStr();
      DisplayUpdate();
      InputError = "";
      DisplayMode = Normal;
      InputResults = "";
      s = "";
  }    
}  

void ProcessKeyScales(int key) {
    ScaleDisp = key;                                                          /* No Display updates until key is pressed */
    cv.clear();
    switch (key) {
      case 'r':
        cv.drawText(0, 0, "Raw Outputs");
        break;
      case 'd':
        cv.drawText(0, 0, "Delta Outputs ");
        break;
      case 'h':
        DisplayScaleCommands();
      case Esc :
        DisplayMode = Normal;
        ClearDisplayUpdateStr();
        DisplayUpdate();
        break;
    }
}

void ProcessKeyCO2Plot (int key) {
  static String keystr = "";
  Serial.println("CO2 Plot Key " + String(key));
  if (key == Esc) {
    if (CO2OnlyFlag == false) {
      ClearDisplayUpdateStr();
      DisplayUpdate();
      DisplayMode = Normal;
    }
    else {
      DisplayPlot();  
    }
  }
  else if (key >= '0' && key <= '9') {
    keystr = keystr + String(key - '0');
  }
  else if (key == CarriageReturn) {
    int newrate = keystr.toInt();
    if (newrate >=0 && newrate <=99) {
      CO2Rate = newrate;
      CO2OnTime = millis();
      Serial.println("New rate is " + String(CO2Rate));
      drawText(Col4, Row13, fluorRed, "Rate is " + String(CO2Rate) + "     ");
    }
    else {
      Serial.println("Input Error key string " + keystr);
    }
    keystr = "";
  }
  else if (key == 'u') {
    ScreenSaverOn = false;
    ScreenSaverStartTime = curtime + ScreenSaverStart;
    Serial.println("u key pressed");
    DisplayPlot();
    delay(1000);
  }
  else if (key == 'a') {
    DisplayAlarms();
  }

  
}
