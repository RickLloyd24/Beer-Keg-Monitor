uint8_t GAIN = 1;               //HX711 GAIN
void ProcessScaleValues(void) {
  static int TimeOutCnt[numscales]; long sv;
  if (ScaleOnFlag == false) return;
  Scale.getData(ScaleReadings);                                                 /* Read all the scales */
  for (int sn = 0; sn < numscales; sn++) {
    sv = ScaleReadings[sn];
    //Serial.print(String(sv) + ", ");
    if (sv == -1) {                                                              /* Time Out Error */
      TimeOutCnt[sn]++;
      if (TimeOutCnt[sn] > 3) {                                                 /* Flag 3 times in row */
        String s = "Scale " + String(sn) + " Tap " + String(sn+1) + " Time Out";
        ProcAlarm(Alarm1, s);
       }
    }   
    else if (sv == 8388608) {                                                  /* Not Connected Error */
      TimeOutCnt[sn]++;
      if (TimeOutCnt[sn] > 3) {                                                 /* Flag 3 times in row */
        String s = "Scale " + String(sn) + " Tap " + String(sn+1) + " Not Connected ";
        ProcAlarm(Alarm2, s);
      }
    }
    else {                                                                        /* Good Scale Value */
      PrevScaleValues[sn] = ScaleValues[sn];
      ScaleValues[sn] = sv;
      TimeOutCnt[sn] = 0;
    }  
  }
  if (DisplayMode == Scales) DisplayScaleValues();
  CalculateGlasses();
}  

void CalculateGlasses() {
  for (int i = 0; i < numscales; i++){  
    float ReadingperOunce = abs(FullKeg[i] - EmptyKeg[i])/(FullKegWeight[i]*16);  /* Calculate Reading per Ounce */
    float OuncesLeft = abs(ScaleValues[i] - EmptyKeg[i])/ReadingperOunce;            /* Calculate new ounces left */
    GlassesLeft[i] = (OuncesLeft - (EmptyKW[i]*16)) / 12;                      /* 12 ounce glasses */
    if (GlassesLeft[i] > 999) GlassesLeft[i] = 999;
    if (GlassesLeft[i] < -99) GlassesLeft[i] = -99;
  }  
}

int calculateDayOfYear(int day, int month, int year) {
  
  // Given a day, month, and year (4 digit), returns 
  // the day of year. Errors return 999.
  
  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  // Verify we got a 4-digit year
  if (year < 1000) {
    return 999;
  }
  
  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (year%4  == 0) {
    if (year%100 != 0) {
      daysInMonth[1] = 29;
    }
    else {
      if (year%400 == 0) {
        daysInMonth[1] = 29;
      }
    }
   }

  // Make sure we are on a valid day of the month
  if (day < 1) 
  {
    return 999;
  } else if (day > daysInMonth[month-1]) {
    return 999;
  }
  
  int doy = 0;
  for (int i = 0; i < month - 1; i++) {
    doy += daysInMonth[i];
  }
  
  doy += day;
  return doy;
}
  
/* Calculate days kegged for all kegs */
void CalcDaysKegged(String date){
  if (TimeSet == false) return;                                          /* if ntp time is not available return */
  //String s = ntp.formattedTime("%m/%d/%y");                              /* Get current date string */
  long myTime = GetDOY(date);                                              /* Get Current Time */
  for (int i = 0; i < numscales; i++) {                                    /* Loop through all Taps */ 
    //s = "Tap " + String(i+1) + " Date " + KeggedDate[i]; Serial.println(s);
    DaysKegged[i] = myTime - GetDOY(KeggedDate[i]);                        /* Set Days Kegged */
    //s = "Days Kegged " + String(DaysKegged[i]); Serial.println(s); 
  }
  return;
}
/* Calculate my Time from date string */
long GetDOY(String date) {
    //unsigned long Epoch = 1672603200;                                            /* Unix Time 1/1/23 12:00 noon */
    //Serial.print("Input Date "); Serial.println(date);
    int m = date.toInt();
    //Serial.print("Month is ");Serial.println(m);
    date = ParseDate(date);
    int d = date.toInt();
    //Serial.print("Day is ");Serial.println(d);
    date = ParseDate(date);
    int y = date.toInt() + 2000;
    //Serial.print("Year is ");Serial.println(y);
    unsigned long doy = calculateDayOfYear(d, m, y);
    //Serial.print("DOY is ");Serial.println(doy);
    unsigned long myTime = (y - EpochYear) * 365 + (doy - 1);
    //Serial.print(" Day is "); Serial.println(myTime);
    return myTime;
}
String ParseDate (String date) {
  for (int i = 0; i < date.length(); i++) {
    if (date[i] == '/') {
      date = date.substring(i + 1,date.length());
      break;
    }
  }
  //Serial.print("Date String is "); Serial.println(date);
  return date;
}

void InitializeScales( void ) {
    int errorflag = 0;
    Scale.begin(ScaleClk, ScaleOut, Gain);
    DisplayPrint("Checking Scales . . . ");
    delay(500);                                                                   //HX711 Settling Time
    Scale.getData(PrevScaleValues);                                                 /* Read all the scales */
    delay(500);
    Scale.getData(ScaleReadings);                                                 /* Read all the scales */
    for (int i = 0; i < 20; i++) {
      errorflag = 0;
      for (int sn = 0; sn < numscales; sn++) {
        if (abs(PrevScaleValues[sn] - ScaleReadings[sn]) > 1000) bitSet(errorflag, sn);
        PrevScaleValues[sn] = ScaleReadings[sn];
      }
      if (errorflag == 0) break;
      delay(500);
      Scale.getData(ScaleReadings);
    }  
    if (errorflag == 0) {
      DisplayPrint("Scales are all good");
    }
    else {
      String s = "";
      for (int sn = 0; sn < numscales; sn++) {
        if (bitRead(errorflag, sn) != 0) s = "Scale " + String(sn) + " is bad";
      }
      DisplayPrint(s);
    }
}
void PowerDownScales (void) {
  digitalWrite(ScalePowerPin, LOW);
  ScaleOnFlag = false;
  delay (11);                                               //Wait for scales to go off
}

void PowerUpScales (void) {
  ScaleOnTime = curtime + 1000;                            //Wait 1 Second before turning On
}
