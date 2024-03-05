/* Process Serial Commands */
void ProcessCmd (String s) {
  int tapn = s.charAt(1) - '0';                                       /* convert char to integer */
  String cmd; String val; int x;                                      /* generate variables */
  if (tapn >= 1 && tapn <= numscales) {                               /* check if it is a valid tap number */
    int sn = tapn - 1;                                                 /* Set Scale number */
/* Process commands with a tap number */    
    cmd = s.substring(2,6);                                           /* extract command from string */
    x = s.length();                                               /* get the length of the string */
    val = s.substring(6, x);                                          /* get the value of the command */   
    // Serial.print ("Cmd, "); Serial.println (cmd); 
    //Serial.print ("Val, "); Serial.println (val); 
    
    if (cmd == "Temp") {                                                  
      temperature[sn] = val.toFloat();                                    
      DataTime = curtime;
      if(sn == 0) ProcessTemp();                                                      
    }  
    else if (cmd == "Glas"){
      GlassesLeft[sn] = val.toInt();
    }
    else if (cmd == "Days"){
      DaysKegged[sn] = val.toInt();
    }  
    else if (cmd == "Alco") {
      Alcohol[sn] = val.toFloat();                                                 
    }  
    else if (cmd == "Styl") { 
      BeerNames[sn] = val;                                              
    }
    else if (cmd == "Alar") { 
      if (tapn == 1) {
        AlarmStr = "";
        AlarmCnt = 0;
      }
      else {
        AlarmStr = val;
        AlarmCnt = 1;
      }
    }
  }
}      
void SendTime(void) {
  String s = "!1Time ";                                            /* Build Command */
  s = s + DateTimeStr(0) + ",";
  Serial2.print(s);                                                
  if (printSerial) {Serialprintln ("Sent: " + s); }
}

void SendWiFi(void) {
  String s = "!1WiFi " + String(RSSIlvl, 1) + ",";                 /* Build Command */
  Serial2.println(s);                                              
  if (printSerial) {Serialprintln ("Sent: " + s); }
} 
/* Process Temperature for alarms */
void ProcessTemp(void){
  if (temperature[0] > 100) {                                              /* Is Freezer On? */
    temperature[0] = temperature[0] - 100;                                 /* Set temperature to true temp */
    freezerOn = 1;                                                         /* set freezer on */
  }
  else {
    freezerOn = 0;                                                         /* set freezer on */
  }
  processAlarms();
}
