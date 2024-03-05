/* This section includes building page and updating variables */
/* Insert Value Function */
void InsertValue(String v, int ptr) {
  if (v.length() < 3) {v = v + blank.substring(0, 3 - v.length());}  /* Pad to 3 chars */
  for (int i = 0; i < v.length(); i++) {                             /* Insert all chars from string */
    HTMLpage[ptr] = v[i];                                            /* Insert char */
    ptr++;                                                           /* move to next position */
  }
}

/* Build Page */
void BuildPage(void) {
/* Insert all current values */ 
  //Serial.println("Building Page");
  InsertValue(       DispTemperature(), indx[0]);                  /* insert current Temperature */ 
  InsertValue(          DispGlasses(0), indx[1]);                  /* insert Tap 1 Glasses Left */ 
  InsertValue(          DispGlasses(1), indx[2]);                  /* insert Tap 2 Glasses Left */ 
  InsertValue(          DispGlasses(2), indx[3]);                  /* insert Tap 3 Glasses Left */ 
  InsertValue(          DispGlasses(3), indx[4]);                  /* insert Tap 4 Glasses Left */ 
  InsertValue(          DispGlasses(4), indx[5]);                  /* insert Tap 5 Glasses Left */ 
  InsertValue(            DaysInKeg(0), indx[6]);                  /* Days kegged Tap 1 */ 
  InsertValue(            DaysInKeg(1), indx[7]);                  /* Days kegged Tap 2  */ 
  InsertValue(            DaysInKeg(2), indx[8]);                  /* Days kegged Tap 3  */ 
  InsertValue(            DaysInKeg(3), indx[9]);                  /* Days kegged Tap 4  */ 
  InsertValue(            DaysInKeg(4), indx[10]);                 /* Days kegged Tap 5  */ 
  InsertValue(       AlcoholPercent(0), indx[11]);                 /* Alcohol Percent Tap 1 */ 
  InsertValue(       AlcoholPercent(1), indx[12]);                 /* Alcohol Percent Tap 2 */ 
  InsertValue(       AlcoholPercent(2), indx[13]);                 /* Alcohol Percent Tap 3 */ 
  InsertValue(       AlcoholPercent(3), indx[14]);                 /* Alcohol Percent Tap 4 */ 
  InsertValue(       AlcoholPercent(4), indx[15]);                 /* Alcohol Percent Tap 5 */ 
  InsertValue(   fstr(BeerNames[0],20), indx[16]);                 /* Tap 1 Name */ 
  InsertValue(   fstr(BeerNames[1],20), indx[17]);                 /* Tap 2 Name */ 
  InsertValue(   fstr(BeerNames[2],20), indx[18]);                 /* Tap 3 Name */ 
  InsertValue(   fstr(BeerNames[3],20), indx[19]);                 /* Tap 4 Name */ 
  InsertValue(   fstr(BeerNames[4],20), indx[20]);                 /* Tap 4 Name */ 
  InsertValue(      sfloat(RSSIlvl, 6), indx[21]);                 /* RSSI Lvl */ 
  InsertValue(        fstr(IPaddr, 15), indx[22]);                 /* IP Addr */ 
  InsertValue(                   myVer, indx[23]);                 /* date and time of compile */ 
  InsertValue(         ConnectStatus(), indx[24]);                 /* Connect Status */ 
  InsertValue(          DateTimeStr(1), indx[25]);                 /* Date and Time */ 
  InsertValue(       DispTempSensors(), indx[26]);                 /* High and Low Temperature */ 
  InsertValue(            DispAlarms(), indx[27]);                 /* Display Alarms */ 
  //Serial.println(HTMLpage);
  return;
}  

/* Find Variable */
int FindVar (int ptr) {
  String v = String(HTMLpage[ptr+1]) + String(HTMLpage[ptr+2]);    /* Get Variable Code */
  //Serial.print("Search Variable "); Serial.println(v);
  if (v == "T0") {return 0;}                                        /* Current Temperature */
  else if (v == "T1") {return 1;}                                   /* Tap 1 Glasses Left */
  else if (v == "T2") {return 2;}                                   /* Tap 2 Glasses Left */
  else if (v == "T3") {return 3;}                                   /* Tap 3 Glasses Left */
  else if (v == "T4") {return 4;}                                   /* Tap 4 Glasses Left */
  else if (v == "T5") {return 5;}                                   /* Tap 5 Glasses Left */
  else if (v == "D1") {return 6;}                                   /* Tap 1 Days in Keg */
  else if (v == "D2") {return 7;}                                   /* Tap 2 Days in Keg */
  else if (v == "D3") {return 8;}                                   /* Tap 3 Days in Keg */
  else if (v == "D4") {return 9;}                                   /* Tap 4 Days in Keg */
  else if (v == "D5") {return 10;}                                  /* Tap 5 Days in Keg */
  else if (v == "P1") {return 11;}                                  /* Percent Alcohol Tap 1 */
  else if (v == "P2") {return 12;}                                  /* Percent Alcohol Tap 2 */
  else if (v == "P3") {return 13;}                                  /* Percent Alcohol Tap 3 */
  else if (v == "P4") {return 14;}                                  /* Percent Alcohol Tap 4 */
  else if (v == "P5") {return 15;}                                  /* Percent Alcohol Tap 5 */
  else if (v == "N1") {return 16;}                                  /* Tap 1 Name */
  else if (v == "N2") {return 17;}                                  /* Tap 2 Name */
  else if (v == "N3") {return 18;}                                  /* Tap 3 Name */
  else if (v == "N4") {return 19;}                                  /* Tap 4 Name */
  else if (v == "N5") {return 20;}                                  /* Tap 5 Name */
  else if (v == "RS") {return 21;}                                  /* RSSI Lvl */
  else if (v == "IP") {return 22;}                                  /* IP Address */
  else if (v == "V1") {return 23;}                                  /* Date and Time of Compile */
  else if (v == "MS") {return 24;}                                  /* Connect Status */
  else if (v == "DT") {return 25;}                                  /* Date and Time */
  else if (v == "TS") {return 26;}                                  /* High and Low Temperature */
  else if (v == "AL") {return 27;}                                  /* Alarm Status */
  else {
    Serial.print("--- Error: Variable not found, Code= ");
    Serial.println(v);
  }
  return 24;
}

/* Fixed String Length */
String fstr(String v, int len) {
  if (v.length() >= len) {                                            /* check if string is too long */
    Serial.println("String is too long ");
    Serial.println(v);
    v = v.substring(0, len - 1);                                     /* cut off end of string */
    return v;
  }  
  //Serial.print("Input String ");Serial.print(v);
  v = v + blank.substring(0, len - v.length());                     /* Pad to len chars */
  //Serial.print(" output string ");Serial.print(v);
  return v;
}
/* float to string */
String sfloat(float var, int len) {
  String s = String(var, 1);                                         /* Convert var to String */
  s = blank.substring(0, len - s.length()) + s;                      /* Pad with leading spaces */
  return s;                                                          /* Return the String */
}
/* int to string */
String sint(int var, int len) {
  String s = String(var);                                            /* Convert var to String */
  s = blank.substring(0, len - s.length()) + s;                      /* Pad with leading spaces */
  return s;                                                          /* Return the String */
}
/* Returns the number of glasses */
String DispGlasses(int j) {  
  int i = GlassesLeft[j];
  String s = "class=\"tg-";
  if (i > 999) {i = 999;}
  if (i < -99) {i = -99;}
  if (i < 10) {
    s = s + "red   \"> " + sint(i, 3);
  }
  else if (i > 20) {
    s = s + "green \"> " + sint(i, 3);
  }
  else {
    s = s + "yellow\"> " + sint(i, 3);
  }
  //Serial.print("Insert Text: "); Serial.println(s);
  return s;
}
/* Display the Connect Connected Status */
String ConnectStatus(void){
  String s = "<span class=\"values\" style=\"color:";
  if (ConnectedFlag == 0) {                                                      /* Is Connect Not connected? */ 
    s = s + "red  \">" + "&emsp; Not Connected to Keg Freezer </span>";                   /* No, Show as red */    
  }
  else {
    s = s + "green\">" + "&emsp; Connected to Keg Freezer     </span>";                    /* Yes, Show as green */
  }
  return s;
}

/* Returns the number of days kegged */
String DaysInKeg (int j) {  
  int i = DaysKegged[j];
  String s = "class=\"tg-";
  if (i > 179) {
    s = s + "red   \"> " + sint(i, 4);
  }
  else if (i > 14) {
    s = s + "green \"> " + sint(i, 4);
  }
  else {
    s = s + "yellow\"> " + sint(i, 4);
  }
  //Serial.print("Insert Text: "); Serial.println(s);
  return s;
}
/* Returns the string to write in the HTML Page */
String AlcoholPercent(int scale) {
  String s = String(Alcohol[scale],1) + "%";
  return s;
}
/* Return High and Low Temperature String */
String DispTempSensors(void) {
  String s = "Temp Sensor 1: " + String(temperature[1], 1) + "&#xBA;F Sensor 2: " + String(temperature[2], 1) + "&#xBA;F";
//           1         2         3         4
//  123456789012345678901234567890123456789012345678
//  Temp Sensor 1: 34.0xxxxxxF Sensor 2: 34.0xxxxxxF
  return s;
}

String DispTemperature (void) {
  String s = sfloat(temperature[0], 5)+ "&#xBA;F";
  if (freezerOn == 1) {
    s = s + " Freezer On ";
  }
  else {
    s = s + " Freezer Off";
  }
  return s;
}

/* Read HTML File Function */
void ReadHTMLPage(void) {
  int PageSize = sizeof(indx_html);
  if (PageSize > MaxBufferSize) {
    String s = "Buffer is not Large enough, add " + String(PageSize-MaxBufferSize) + " bytes";
    Serial.println(s);
    s.toCharArray(HTMLpage,s.length());
    return;
  }
  Serial.print(MaxBufferSize - PageSize); Serial.println(" Bytes available in HTML buffer");
  for (int i = 0; i < PageSize; i++) {
    HTMLpage[i] = indx_html[i];
  }
  for (int i = 0; i < PageSize; i++) {
    char c = HTMLpage[i];
    if(!isAscii(c)) {
      if (!((c == '\r')  || (c == '\n'))) {
        Serial.print("Non ASCII charactor found: "); Serial.println((HTMLpage[i]) - '0');  
        c = ' ';
      }
    }
    if(c == '%') {
      indx[FindVar(i)] = i;
    }
    //Serial.print(HTMLpage[i]);
  }
  HTMLpage[PageSize] = '\0';
  //Serial.println();
  return;
}
/* Display Alarm Status */
String DispAlarms(void){
  String s = "class=\"values\" style=\"color:";
  if (AlarmCnt == 1) {                                                  
    s = s + "red  \">" + AlarmStr + "</span>";  
  }
  else {
    s = s + "green\">" + "No Alarms  </span>";                   
  }
  //Serial.println(s);
  return s;
}
