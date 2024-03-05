void ProcessCommand(String s) {
  String SubString = s.substring(0, 3);
  SubString.toLowerCase();
  if (SubString == "tap") {
    int tapn = s.charAt(4) - '0';                                       /* convert char to integer */
    if (tapn < 1 || tapn > numscales) {
      InputError = "Tap Error: range 1 to Number of Taps";
      Serial.println(InputError);
      tapn = 1;
    }
    int snum = tapn - 1;  
    SubString = s.substring(6, 9);
    if (SubString == "alc") {                                           /* Command Alcohol */
      SubString = s.substring(9, s.length());
      Alcohol[snum] = SubString.toFloat();
      InputResults = "Tap " + String(tapn) + " New Alcohol % = " + String(Alcohol[snum]);
      Serial.println(InputResults);
    }
    else if (SubString == "sty") {                                      /* Command Style */
      SubString = s.substring(12, s.length());
      if (SubString.length() > StyleLen) {
        InputError = "Style Name is too long";
      }
      else {
      BeerNames[snum] = SubString;
      InputResults = "Tap " + String(tapn) + " New Style = " + BeerNames[snum];
      Serial.println(InputResults);
      }
    }
    else if (SubString == "dat") {                                      /* Command Kegged Date */
      SubString = s.substring(10, s.length());
      KeggedDate[snum] = SubString;
      InputResults = "Tap " + String(tapn) + " New Kegged Date = " + KeggedDate[snum];
      Serial.println(InputResults);
    }
    else if (SubString == "emp") {                                      /* Command Empty Keg Weight */
      SubString = s.substring(15, s.length());
      EmptyKW[snum] = SubString.toFloat();
      InputResults = "Tap " + String(tapn) + " New Empty Keg Weight = " + String(EmptyKW[snum], 1);
      Serial.println(InputResults);
    }
    else if (SubString == "cal") {                                      /* Command Cal */
      CalibratingFlag = snum;
      SubString = s.substring(10, s.length());
      CalWeight = SubString.toFloat();
      InputResults = "Starting Calibration Weight " + String(CalWeight, 1);
      Serial.println(InputResults);
    }
    else InputError = "Invalid Tap Input: " + s;
  }  
  else if (SubString == "tem") {                                      /* Command Temperature Bias */
    int tmpn = s.charAt(5) - '0';
    Serial.print("tmpn = "); Serial.println(tmpn);
    if (tmpn < 1 || tmpn > 2) {
      InputError = "Temp number must be 1 or 2";
    }
    else {
      SubString = s.substring(11, s.length());
      Serial.print("Value Substring "); Serial.println(SubString);
      TemperatureBias[tmpn] = SubString.toFloat();
      InputResults = "Temp #1 bias: " + String(TemperatureBias[1],1) + " #2 bias: " + String(TemperatureBias[2],1);
      Serial.println(InputResults);
    }  
  }
  else if (SubString == "ala") {                                         /* Command Alarms */
    Serial.println("Alarms command Received");
    DisplayAlarms();
    DisplayMode = Alarm;
  }
  else if (SubString == "sca") {                                         /* Command Scales */
    Serial.println("Scales command Received");
    DisplayScaleCommands();
    DisplayMode = Scales;
  }
  else if (SubString == "sav") {                                         /* Save Command */
    Serial.println("Save command Received");
    WriteConfigFile();
    InputResults = "Current Configuration saved to file";
  }
  else if (SubString == "dat") {                                      /* Current Date Command */
    SubString = s.substring(4, s.length());
    unsigned long CurrentTime = CalcUnixTime(SubString);
    setTime(CurrentTime); 
    TimeSet = true;
    CalcDaysKegged(SubString);
    SubString = "Days Kegged = ";
    for (int j = 0; j < numscales; j++){
      SubString = SubString + String(DaysKegged[j]) + ", ";
    }
    InputResults = SubString;
    Serial.println(InputResults);
  }
  else if (SubString == "goa") {                                      /* Input new Temperature Goal */
    SubString = s.substring(4, s.length());
    Tempgoal = SubString.toInt();
    InputResults = "New Temperature Goal " + String(Tempgoal, 1);
    ClearTempStatistics();
    Serial.println(InputResults);
  }  
  else InputError = "Invalid Input: " + s;
}
/* Calculate date and time mm/dd/yy hh:mm*/
unsigned long CalcUnixTime(String datetime){
  int Hour = 0; int Min = 0;
  
  int myTime = GetDOY(datetime);                                              /* Get Current DOY */
  Serial.print("DOY is ");Serial.println(myTime); 
  ParseTime(datetime, Hour, Min);
  Serial.print("Time ");Serial.print(Hour);Serial.print(":");Serial.println(Min);
  unsigned long Unix = EpochUnix + myTime * 86400 + Hour * 3600 + Min * 60 + TimeZone(myTime);
  
  return Unix;
}

String ParseTime (String Time, int &Hour, int &Min) {
  String SubString = ""; int space = 0;
  for (int i = 0; i < Time.length(); i++) {
    if(Time[i] == ' ') space = i;  
    if (Time[i] == ':') {
      SubString = Time.substring(space, i);
      Hour = SubString.toInt(); 
      SubString = Time.substring(i + 1, Time.length());
      Min = SubString.toInt();
      break;
    }
  }
  //Serial.print("Date String is "); Serial.println(date);
  return Time;
}
/* Date/Time Formatter */
String DateTimeStr (void) {
  String s =  DayofWeek(weekday()) + " " +String(month()) + "/" + String(day()) + "/" + String(year()) + "  ";
  s = s + fStr(hourFormat12()) + ":" + fStr(minute());
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
unsigned long TimeZone(int doy) {
   if (doy > 70 && doy < 308) {                                    /* Daylight Savings Time */
    return ((TimeZoneOffset - 1) * 3600);
   }
   else {
    return (TimeZoneOffset*3600);                                               /* Standard Time */
   }
}
String fStr (int i) {
  if (i < 10) {
    return ("0" + String(i));
  }
  else {
    return String(i);
  }
}
