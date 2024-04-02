void ProcessSerial2(String s) {
  String val; 
  int tapn = s.charAt(1) - '0';
  int snum = tapn - 1;
  if (snum < 0 || snum > 4) {
    //String s = "Serial2: Invalid Scale number " + String(snum);
    ProcAlarm(Alarm0, s);
  }
  else {
    String cmd; String val; int x;
    cmd = s.substring(2,6);                                           /* extract command from string */
    x = s.length();
    val = s.substring(6, x);                                          /* get the value of the command */   
    //Serial.println("Cmd " + cmd + " val " + val);
    if (cmd == "Time") {                                                /* Time Command? */
      DateTime = val;                 
      CalcDaysKegged(DateTime);                                         
    }
    else if (cmd == "WiFi") {                                        /* WiFi cmd? */   
      //RSSIlvl = val.toFloat();      
      //PrintWiFi();               
    }
    else if (cmd == "Conf") {                                        /* Configuration Cmd? */
      SendConf();
    }
  }
}

void SendTemperature(){
  for (int i = 0; i <= numTS; i++) {
    float t = Temperature[i];
    if (FreezerState == 1 && i == 0) {t = t + 100;}                                  /* Add 100 to show Freezer is on */
    String s = String("&" + String(i+1) + "Temp ") + String(t, 1) + ";";             /* Add the temperature value to string */
    Serial2.print (s);                                                               /* Send Command */
  }  
}
/* Send Glasses to Uno board */  
void SendGlasses (void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    int glassesleft = GlassesLeft[i];
    if(glassesleft > 99)  glassesleft = 99;                           
    if(glassesleft < -99)  glassesleft = -99;
    String s = "&" + String(i+1) + "Glas ";                                 /* Build command */
    s = s + String(glassesleft) + ";";                                   /* Add glasses */
    Serial2.print (s);                                                    /* Send Command */
    if (printSerial) Serial.println ("Sent: " + s);                                                    /* Send Command */
  }
}
void SendDaysKegged(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Days ";                                 /* Build command */
    s = s + String(DaysKegged[i]) + ";";                                    /* Add Days Kegged */
    Serial2.print (s);                                                    /* Send Command */
  }
}
void SendAlcohol(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Alco ";                                 /* Build command */
    s = s + String(Alcohol[i], 1) + ";";                                    /* Add Percent Alcohol */
    Serial2.print (s);                                                    /* Send Command */
  }
}
void SendStyle(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Styl ";                                 /* Build command */
    s = s + BeerNames[i] + ";";                                    /* Add Percent Alcohol */
    Serial2.print (s);                                                    /* Send Command */
  }
}

void SendAlarms(void) {
  if (AlarmCnt == 0) {
    Serial2.print("&1Alar   ;");                                            /* No alarms */
  }  
  else {  
    int i = 2; int ptr = 0;
    for (int j = 0; j < AlarmStr.length(); j++){
      if (AlarmStr[j] == ';') {
        String s = "&" + String(i) + "Alar ";       
        s = s + AlarmStr.substring(ptr, j) + ";";
        Serial2.print(s);
        i++; ptr = j + 1; s = "";
        if (i > 4) return;
      }
    } 
  }
  return;
}

void SendConf(void) {
  String s = "&1Conf ";
  s = s + ReadConfigFile() + ";";
  Serial2.print(s);
  if (printSerial) Serial.println(s);
}
