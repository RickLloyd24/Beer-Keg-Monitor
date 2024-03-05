String s = "";

/* Process key strokes */
void ProcessKey(char key) {
  if (DisplayMode == Scales) {
    ProcessKeyScales(key);
  }
  else if (DisplayMode == Command) {
    ProcessKeyCommand(key);
  }
  else if (DisplayMode == Alarm) {
    if (key == Esc) {
      DisplayUpdate();
      DisplayMode = Normal;
    }
  }
  else if (DisplayMode == Normal) {
    //Serial.print("key Pressed "); Serial.println(key, DEC);
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
        DisplayUpdate();
        break;
    }
}          
