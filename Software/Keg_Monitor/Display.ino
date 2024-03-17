/* Display Update */
void DisplayUpdate(void) {
  int color = 0;
  //Serial.println("Entered DisplayUpdate Function");
  cv.clear();
  setPenColor64(lemon);
  cv.drawText(0, Row0,    "       Rick's Beer Keg Monitor");  
  setPenColor64(electricBlue);
  cv.drawTextFmt(0, Row1, "     Average Temperature %4.1f F", Temperature[0]);
  setPenColor64(brightWhite);
  cv.drawText(Col0, Row2,   "Tap Glasses       Style         Alc Days"); 
  setPenColor64(jigglypuff);
  int cRow = Row3;
  for (int j = 0; j < numscales; j++) {
    setPenColor64(soulside);cv.drawTextFmt(Col1, cRow, "%d ", j+1);
    if (GlassesLeft[j] > 30) cv.setPenColor(Color::BrightGreen);
    else if (GlassesLeft[j] < 10) cv.setPenColor(Color::BrightRed);
    else setPenColor64(pilaYellow);
    cv.drawTextFmt(Col4, cRow, " %5.1f ", GlassesLeft[j]);
    setPenColor64(orange);
    drawTextStr(Col12, cRow, BeerNames[j]);
    setPenColor64(turquoise);
    cv.drawTextFmt(Col32, cRow,   "%3.1f  %3d", Alcohol[j], DaysKegged[j] );
    cRow = cRow + Row;
  }
  setPenColor64(blueLobster);
  if (numTS == 1) {
    cv.drawText(0, Row8, "Temp "); 
    setPenColor64(alpineWhite);
    cv.drawTextFmt(Col8, Row8, "%4.1f F ", Temperature[1]);
  }
  else if (numTS == 2) {
    cv.drawText(0, Row8, "Temp #1 "); 
    color = (UsingSensor  == 1) ? alpineWhite : blueLobster; setPenColor64(color); 
    cv.drawTextFmt(Col8, Row8, "%4.1f F ", Temperature[1]);
    color = (UsingSensor  == 2) ? alpineWhite : blueLobster; setPenColor64(color); 
    cv.drawTextFmt(Col16, Row8, "#2 %4.1f F", Temperature[2]);
    cv.drawTextFmt(Col27, Row8, "Delta %4.1f F", Temperature[1] - Temperature[2]);
  }
  else {
    cv.drawText(0, Row8, "Temp #1 "); 
    color = (UsingSensor  == 1) ? alpineWhite : blueLobster; setPenColor64(color); 
    cv.drawTextFmt(Col8, Row8, "%4.1f F ", Temperature[1]);
    color = (UsingSensor  == 2) ? alpineWhite : blueLobster; setPenColor64(color); 
    cv.drawTextFmt(Col16, Row8, "#2 %4.1f F", Temperature[2]);
    color = (UsingSensor  == 3) ? alpineWhite : blueLobster; setPenColor64(color); 
    cv.drawTextFmt(Col27, Row8, "#3 %4.1f F", Temperature[3]);
  }
  setPenColor64(brown);
  cv.drawTextFmt(0, Row9,   " Max %4.1f Min %4.1f ", HighTemp, LowTemp);    
  if (digitalRead(FrigControlPin) == 1) {
    setPenColor64(shyGuyRed);
    cv.drawTextFmt(Col20, Row9, "Freezer On   %2d", FreezerOnPercent);
  }
  else {
    setPenColor64(shyMoment);
    cv.drawTextFmt(Col20, Row9, "Freezer Off  %2d", 100 - FreezerOnPercent);
  }
  setPenColor64(blobGreen);
  cv.drawText   (Col36, Row9, "%");
  if (AlarmCnt == 0) {
    setPenColor64(yoshiGreen); cv.drawText(0, Row10, "No Alarms");
  }  
  else {
    setPenColor64(mysticRed); cv.drawTextFmt(0, Row10, "Alarms set %d ", AlarmCnt);
  }
  setPenColor64(ultimatePink);
  drawTextStr(Col13, Row10, DateTime); 
  setPenColor64(brandywine);
  int sn = CurrentTap - 1;
  //Serial.print("Current Tap ");Serial.println(CurrentTap);
  cv.drawTextFmt(Col0, Row11, "Tap %1d   Full %8d Weight %4.1f lbs", CurrentTap, FullKeg[sn], FullKegWeight[sn]);  
  long delta = ScaleValues[sn]- PrevScaleValues[sn];
  float deltaoz = delta / (abs(FullKeg[sn] - EmptyKeg[sn])/(FullKegWeight[sn]*16));
  cv.drawTextFmt(Col0, Row12, "     Current %8d Delta %6.2f oz", ScaleValues[sn], deltaoz);
  cv.drawTextFmt(Col0, Row13, "       Empty %8d Weight %4.1f lbs", EmptyKeg[sn], EmptyKW[sn]);  
}

void DisplayCommands(void) {
  //Serial.println("Entered DisplayCommands Functions");
  cv.clear();
  cv.setPenColor(Color::BrightWhite);
  cv.drawText(0, Row0, "Tap i Alc f.f %");
  cv.drawText(0, Row1, "Tap i Style ssssssssss");
  cv.drawText(0, Row2, "Tap i Date mm/dd/yy Kegged");
  cv.drawText(0, Row3, "Tap i Empty Keg ff.f pounds");
  cv.drawText(0, Row4, "Tap i Cal ff.f pounds (0=Empty)");
  cv.drawText(0, Row5, "Alarms Display/Clear");
  cv.drawText(0, Row6, "Scales Display Values");
  cv.drawText(0, Row7, "Save to file");
  cv.drawText(0, Row8, "Date mm/dd/yy hh:mm");
  cv.drawTextFmt(0, Row9, "Temp i Bias ff.f (%4.1f, %4.1f, %4.1f)", TempBias[1], TempBias[2], TempBias[3]);
  cv.drawTextFmt(0, Row10, "Goal xx (Current %2d) degrees", Tempgoal);
  // Row 11 Status
  // Row 12 Error
  if (InputError != "") {
    Serial.println(InputError);
    cv.setPenColor(Color::Red);
    drawTextStr(0, Row12, InputError);
  }
  if (InputResults != "") {
    DisplayCmdStatus(InputResults);
  }
  cv.setPenColor(Color::Yellow);
  cv.drawRectangle(0, 222, 320 - 1, 240 - 1);
  cv.drawText(3, Row14,  "_");
}

void DisplayCmdStatus(String s) {
  ClearRow(Row11);
  cv.setPenColor(Color::Green);
  drawTextStr(0, Row11, s);
}
void DisplayRow14(String s) {
  ClearRow(Row14);
  cv.setPenColor(Color::Yellow);
  drawTextStr(0, Row14, s);
}

/* Display key pressed in bottom rectangle */
void DisplayKeys(String s) {
  //cv.setPenColor(Color::Black);
  //cv.drawText(3, Row14, "                ");
  cv.setBrushColor(Color::Black);
  cv.fillRectangle(1, 224 - 1, 320 - 2, 240 - 2);
  cv.setPenColor(Color::BrightWhite);
  drawTextStr(3, Row14, s);
  cv.drawText(s.length()*Col+3, Row14, "_");
}

void DisplayAlarms (void) {
  //Serial.println("Entered DisplayAlarms Function");
  cv.clear();
 if (AlarmCnt == 0) {
    cv.setPenColor(Color::Green);
    cv.drawText(0, Row0, "No Alarms");
 }
 else {
    cv.setPenColor(Color::BrightWhite);
    cv.drawText(0, Row0, "Alarms");
    cv.setPenColor(Color::Red);
    int cRow = Row1;
    int ptr = 0;
    for (int j = ptr; j < AlarmStr.length(); j++){
      if (AlarmStr[j] == ';') {
        String s = AlarmStr.substring(ptr,j);
        Serial.println(s);
        drawTextStr(0, cRow, s);
        ptr = j + 1;
        cRow = cRow + Row;
      }
    }
    Serial2.println("&1Alar ,");                               /* No Alarms */
    AlarmStr = ""; AlarmCnt = 0; AlarmFlag = 0;
  }  
}

void ClearRow(int row) {
  cv.setBrushColor(Color::Black);
  cv.fillRectangle(0, row, 320, row+Row);
}

void DisplayScaleCommands(void){
  cv.clear();
  cv.setPenColor(Color::BrightWhite);
  cv.drawText(0, 0, "Scale Commands");
  cv.drawText(0, Row1, "Display raw values - r");
  cv.drawText(0, Row2, "Display delta values - d");
  cv.drawText(0, Row3, "Display Help - h");
  cv.drawText(0, Row4, "Exit - Esc");
  ScaleDisp = ' ';                                                       /* No Display Update until key is pressed */
}

void DisplayScaleValues(void) {
static int curRow = Row0;
static char OldScaleDisp = 'x';

    if (ScaleDisp != OldScaleDisp) {                                         /* Change in Display */
      OldScaleDisp = ScaleDisp;
      curRow = Row0;
    }
    curRow = curRow + Row;
    if(curRow > MaxRows*Row) curRow = Row1;
    switch (ScaleDisp) {
      case 'r':                                                                 /* Raw Values */
        ClearRow(curRow);
        for (int j = 0; j<numscales; j++){
          cv.drawTextFmt(j*Col*8, curRow, "%7d", ScaleValues[j]);
        }
        if(curRow < MaxRows*Row) ClearRow(curRow + Row);
        break;
      case 'd':                                                                  /* Delta Outputs */
        ClearRow(curRow);
        for (int j = 0; j<numscales; j++){
          cv.drawTextFmt(j*Col*8, curRow, "%7d", (ScaleValues[j]- PrevScaleValues[j]));
        }  
        if(curRow < MaxRows*Row) ClearRow(curRow + Row);
        break;
      case 'h':
        DisplayScaleCommands();                                                        
        break;
    }      
}    
    
void drawTextStr(int X, int Y, String S) {
  int len = S.length() + 1;
  char buf[len];
  S.toCharArray(buf, len);
  for(int i = 0; i < len; i++) {
    cv.drawChar(X + i*8, Y, buf[i]);     
  }
}
void DisplayPrint(String s) {
  static int cRow = Row0;
  drawTextStr(0, cRow, s);
  cRow = cRow + Row;
  Serial.println(s); 
}

void ProcAlarm(int alarmNum, String a) {  
  if (bitRead(AlarmFlag, alarmNum) == 0){                                   /* Alarm not set add */
    Serial2.println("&" + String(AlarmCnt + 2) + "Alar " + a + ",");
    Serial.println(a);
    bitSet(AlarmFlag, alarmNum);
    AlarmCnt++;
    if (AlarmCnt < (MaxRows - 2)) {
      AlarmStr = AlarmStr + a + ";";
    }
  }  
}

void setBrushColor64(int num) {
  int R; int G; int B;
  RGBFunction(num, R, G, B);
  cv.setBrushColor  (R, G,  B);
}
void setPenColor64(int num) {
  int R; int G; int B;
  RGBFunction(num, R, G, B);
  cv.setPenColor  (R, G,  B);
}
/* This function returns the RGB values for the 64 colors */
/* colornum is from 0 to 63 */
void RGBFunction(int num, int &R, int &G, int &B){
  if (num > 63) return;
  R = int(num / 16) * 64;
  G = int((num % 16) / 4) * 64;
  B = (num % 4) * 64;
  //Serial.println("Color Num= " + String(num) + " RGB " + String(red) + ", " + String(green) + ", " + String(blue));
}
