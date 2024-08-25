void ProcessCommand(String s) {
  //s.toLowerCase();
  String SubString = s.substring(0, 3);
  Serial.println("Cmd " + String(SubString));
  if (SubString == "Tap") {
    int tapn = s.charAt(4) - '0';                                       /* convert char to integer */
    int snum = tapn - 1;  
    InputError = "";
    if (tapn < 1 || tapn > numscales) {
      InputError = "Tap Error: range 1 to Number of Taps";
      Serial.println(InputError);
      tapn = 1;
    }
    SubString = s.substring(6, 9);
    if (SubString == "Alc") {                                           /* Command Alcohol */
      SubString = s.substring(9, s.length());
      Alcohol[snum] = SubString.toFloat();
      InputResults = "Tap " + String(tapn) + " New Alcohol % = " + String(Alcohol[snum]);
      Serial.println(InputResults);
    }
    else if (SubString == "Sty") {                                      /* Command Style */
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
    else if (SubString == "Dat") {                                      /* Command Kegged Date */
      SubString = s.substring(10, s.length());
      KeggedDate[snum] = SubString;
      InputResults = "Tap " + String(tapn) + " New Kegged Date = " + KeggedDate[snum];
      Serial.println(InputResults);
    }
    else if (SubString == "Emp") {                                      /* Command Empty Keg Weight */
      SubString = s.substring(15, s.length());
      EmptyKW[snum] = SubString.toFloat();
      InputResults = "Tap " + String(tapn) + " New Empty Keg Weight = " + String(EmptyKW[snum], 1);
      Serial.println(InputResults);
    }
    else if (SubString == "Cal") {                                      /* Command Cal */
      SubString = s.substring(10, s.length());
      CalWeight = SubString.toFloat();
      if (CalWeight == 0) {
        EmptyKeg[snum] = ScaleValues[snum];
        InputResults = "Calibration Empty Keg Complete";
      }  
      else {
        FullKegWeight[snum] = CalWeight;
        FullKeg[snum] = ScaleValues[snum];
        InputResults = "Calibrate Full Keg Complete";  
      }
      
      Serial.println(InputResults);
    }
    else InputError = "Invalid Tap Input: " + s;
  }  
  else if (SubString == "Tem") {                                      /* Command Temperature Bias */
    int tmpn = s.charAt(5) - '0';
    Serial.print("tmpn = "); Serial.println(tmpn);
    if (tmpn < 1 || tmpn > 3) {
      InputError = "Temp number must be 1 - 3";
    }
    else {
      SubString = s.substring(11, s.length());
      Serial.print("Value Substring "); Serial.println(SubString);
      TempBias[tmpn] = SubString.toFloat();
      Temperature[tmpn] = Temperature[tmpn] + TempBias[tmpn];
      InputResults = "Temp bias: #1 " + String(TempBias[1],1) + " #2 " + String(TempBias[2],1) +  + " #3 " + String(TempBias[3],1);
      Serial.println(InputResults);
    }  
  }
  else if (SubString == "Rat") {                                         /* Command Alarms */
    SubString = s.substring(4, s.length());
    CO2Rate = SubString.toInt();
    Serial.println("CO2 Rate of " + String(CO2Rate) + " command Received");
    CO2OnTime = millis();
    InputResults = "CO2 Rate Set to " + String(CO2Rate) + " minutes";
  }
  else if (SubString == "Sca") {                                         /* Command Scales */
    Serial.println("Scales command Received");
    DisplayScaleCommands();
    DisplayMode = Scales;
  }
  else if (SubString == "Sav") {                                         /* Save Command */
    Serial.println("Save command Received");
    WriteConfigFile();
    InputResults = "Current Configuration saved to file";
  }
  else if (SubString == "Dat") {                                      /* Current Date Command */
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
  else if (SubString == "Goa") {                                      /* Input new Temperature Goal */
    SubString = s.substring(4, s.length());
    Tempgoal = SubString.toInt();
    InputResults = "New Temperature Goal " + String(Tempgoal, 1);
    ClearTempStatistics();
    Serial.println(InputResults);
  }
  else if (SubString == "Set") {                                      /* Set Secondary */
    if (digitalRead(PrimarySecondaryPin) == Primary) {
      digitalWrite(PrimaryOnOffPin, LOW);                             /* Turn off Freezer */ 
      delay(1000);
      digitalWrite(PrimarySecondaryPin, Secondary);
      InputResults = "Set to Secondary" ;
    }  
    else {  
      digitalWrite(SecondaryOnOffPin, LOW);                           /* Turn off Freezer */ 
      delay(1000);
      digitalWrite(PrimarySecondaryPin, Primary);
      InputResults = "Set to Primary" ;
    }
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
