// Initialize WiFi
void InitWiFi(void) {
  uint8_t newMACAddress[6] {0x08, 0x3a, 0xf2, 0xb7, 0xbb, 0x94};       /* Keg Monitor 192.168.0.182 */
  //Mac Address 08:3A:F2:B7:BB:94
  //New Mac Address 08:3A:F2:B7:C1:0C
  
  WiFi.persistent( false );                                             /* Turn off saving parameters in FLASH */
  String host = String(newhostname);
  WiFi.hostname(host.c_str());                                          /* set new host name */
  WiFi.mode(WIFI_STA);                                                  /* set this module operates as a station */
  WiFi.begin(ssid, password);                                           /* connect to Wifi */ 
  delay(500);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  long timeout = millis(); WiFiflag = true;
  while (WiFi.status() != WL_CONNECTED) {                               /* loop until connected */
    delay(500);
    Serial.print(".");
    if (millis() - timeout > 300000) {                                  /* If can't connect in 5 minutes exit */
      Serial.println("WiFi Connection Failure");                        /* Set Critical Status */
      WiFi.printDiag(Serial);                                           /* Print out Diagnostics */
      WiFiflag = false;                                                 /* Set Not connected */  
      break;                                                            /* Exit Loop */
    }
  }
  if (WiFiflag) {
    Serial.println ("Connect Sucessful!"); 
    if (!MDNS.begin(host)) {
      Serial.println("Error on MDNS responder!");                       /* Set Critical Error Status */
    }
    Serial.print("MDNS started with name: "); Serial.println(host);
    WiFi.setAutoReconnect(true);                                            /* Try to reconnect, does not always work */
    Serial.print ("Mac Address "); Serial.println (WiFi.macAddress());      /* Pring new Mac Address */
    Serial.print("IP Address: ");Serial.println(WiFi.localIP().toString());   
    RSSIlvl = WiFi.RSSI();                                           /* Get current signal level */
    IPaddr = WiFi.localIP().toString();                              /* Get the IP Address */
  }  
  return;
}


/* Write to display and serial during boot */
void Serialprintln(String s) {
  WebSerial.println(s);
  Serial.println(s); 
}
/* Check if there are any critical alarms */
void processAlarms(void) {
  if (AlarmFlag == false) {                                                /* Has alarm message already been sent? */
    if (temperature[0] > Maxtemp) {                                          /* Is Freezer too hot? */
      String s = "Keg Monitor: Temperature is too hot, " + String(temperature[0], 1) + " degrees.";
      Serialprintln(s);
      eMail(s, 3);                                                        /* Send Alarm */
      AlarmFlag = true;                                                   /* Set flag Alarm has been sent */
    }
    if (temperature[0] < Mintemp) {                                          /* Is Freezer too cold? */
      String s = "Keg Monitor: Temperature is too cold, " + String(temperature[0], 1) + " degrees.";
      Serialprintln(s);
      eMail(s, 3);                                                        /* Send Alarm */
      AlarmFlag = true;                                                   /* Set flag Alarm has been sent */
    }
    if (curtime > DataTime + HOURMILLS) {
      String s = "No commincation with Keg Freezer for one hour";
      Serialprintln(s);
      eMail(s, 3);                                                        /* Send Alarm */
      AlarmFlag = true;                                                   /* Set flag Alarm has been sent */
    }
  }
  else {                                                                  /* Alarm has been sent */
    if (temperature[0] < Maxtemp  && temperature[0] > Mintemp && ConnectedFlag) {            
      AlarmFlag = false;                                                    /* Clear Alarm flag */
    }
  }
}
/* Set Real Time Clock to Local Time */
boolean SetRTCTime(void) {
  struct tm timeinfo;
  int trys = 0;
  
  while (trys < 3) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if (getLocalTime(&timeinfo)) {
      Serial.println("Got Time from ntpServer");
      //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);  
      return true;
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer2);
    if (getLocalTime(&timeinfo)) {
      Serial.println("Got Time from ntpServer2");
      //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);  
      return true;
    }
    trys++; if (trys >= 3) return false;
    delay(500);
  }
  return false;
}  
/* Date/Time Formatter */
String DateTimeStr (int DOW) {
  String s = ""; char buf[10];
  if (ValidTime) {
    if (DOW == 1) {s =  DayofWeek(weekday());}  
    sprintf(buf, " %d/%d/%d ", month(), day(), year()-2000);
    s = s + String(buf);
    sprintf(buf, "%d:%02d ", hourFormat12(), minute());
    s = s + String(buf);
    if (isAM()) {
      s = s + "AM";
    }
    else {
      s = s + "PM";
    }
  }
  else {
    s = "Time Not Available";  
  }  
  //Serialprintln(s);
  return s;
}
String DayofWeek(int Day) {
  // Day of the week (1-7), Sunday is day 1
  if (Day == 1) return "Sunday";
  if (Day == 2) return "Monday";
  if (Day == 3) return "Tuesday";
  if (Day == 4) return "Wednesday";
  if (Day == 5) return "Thursday";
  if (Day == 6) return "Friday";
  if (Day == 7) return "Saturday";
}
  
