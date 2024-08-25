/* Initialize WiFi */
boolean InitWiFi(void) {
  WiFi.persistent( false );                                             /* Turn off saving parameters in FLASH */
  WiFi.mode(WIFI_STA);                                                  /* set this module operates as a station */
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to " + String(ssid));
  long timeout = millis(); WiFiflag = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (millis() - timeout > 60000) {                                  /* If can't connect in 1 minutes exit */
      Serial.println();
      WiFi.printDiag(Serial);                                           /* Print out Diagnostics */
      WiFiflag = false;                                                 /* Set Not connected */  
      return false;
    }
  }
  Serial.println();
  if (WiFiflag) {
    //WiFi.setAutoReconnect(true);                                          /* Try to reconnect, does not always work */
    Serial.print ("Mac Address "); Serial.println (WiFi.macAddress());    /* Pring new Mac Address */
    String IPaddr = WiFi.localIP().toString();
    Serial.println("IP Address: " + IPaddr);   
    RSSIlvl = WiFi.RSSI();
  }  
  return true;
}

/*
  For times east of the Prime Meridian use 0 through 12
  For times west of the Prime Meridian negative offset 0 through -12.
  Ex. American/Denver GMT would be -6. -6 * 60  */
boolean SetRTCTime(void) {
    struct tm timeinfo;
    if (WiFiflag) {
      int trys = 0;
      while (trys < 3) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        if (getLocalTime(&timeinfo)) {
          Serial.println("Got Time from ntpServer");
          Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
          setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);  
          return true;
        }
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer2);
        if (getLocalTime(&timeinfo)) {
          Serial.println("Got Time from ntpServer2");
          Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
          setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);  
          return true;
        }
        trys++; if (trys >= 3) return false;
        Serial.println("Trying again to get time");
        delay(2000);
      }
    }  
    return false;
}  

/* Date/Time Formatter */
String DateTimeStr (void) {
  String s =  String(month()) + "/" + String(day()) + "/" + String(year() - 2000) + " ";
  s = s + fStr(hourFormat12()) + ":" + fStr(minute());
  s = s + " " + DayofWeek(weekday());
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
