void ProcessSerial2(String s) {
  String val; 
  int tapn = s.charAt(1) - '0';
  int snum = tapn - 1;
  if (snum < 0 || snum > 4) {
    String s = "Serial2: Invalid Scale number " + String(snum);
    ProcAlarm(Alarm0, s);
    Serial.println(s);
  }
  else {
    String cmd; String val; int x;                                      /* generate variables */
    cmd = s.substring(2,6);                                           /* extract command from string */
    x = s.length() - 1;                                                   /* get the length of the string */
    val = s.substring(6, x);                                          /* get the value of the command */   
    if (cmd == "Time") {                                            /* Time Command? */
      DateTime = val;                                                    /* Save Value */
      CalcDaysKegged(DateTime);                                         
    }
    else if (cmd == "WiFi") {                                            /* WiFi Command? */
      //RSSIlvl = val.toFloat();                                           /* Get Value */
      //PrintWiFi();                                                       /* Print Value */
    }
  }
}

void SendTemperature(){
  for (int i = 0; i < 3; i++) {
    float t = Temperature[i];
    if (FreezerState == 1 && i == 0) {t = t + 100;}                                  /* Add 100 to show Freezer is on */
    String s = String("&" + String(i+1) + "Temp ") + String(t, 1) + ",";             /* Add the temperature value to string */
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
    s = s + String(glassesleft) + ",";                                   /* Add glasses */
    Serial2.println (s);                                                    /* Send Command */
  }
}
void SendDaysKegged(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Days ";                                 /* Build command */
    s = s + String(DaysKegged[i]) + ",";                                    /* Add Days Kegged */
    Serial2.println (s);                                                    /* Send Command */
  }
}
void SendAlcohol(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Alco ";                                 /* Build command */
    s = s + String(Alcohol[i], 1) + ",";                                    /* Add Percent Alcohol */
    Serial2.println (s);                                                    /* Send Command */
  }
}
void SendStyle(void) {
  for (int i = 0; i < numscales; i++){                                      /* loop through all values */
    String s = "&" + String(i+1) + "Styl ";                                 /* Build command */
    s = s + BeerNames[i] + ",";                                    /* Add Percent Alcohol */
    Serial2.println (s);                                                    /* Send Command */
  }
}

void SendAlarms(void) {
  if (AlarmCnt == 0) {
    Serial2.println("&1Alar ,");                                            /* No alarms */
  }  
  else {  
    for (int j = 0; j < AlarmStr.length(); j++){
      if (AlarmStr[j] == ';') {
        String s = "&2Alar ";       
        s = s + AlarmStr.substring(0, j - 1) + ",";
        Serial2.println(s);                                               /* Send 1st Alarm */
        return;
      }
    } 
  }   
}
