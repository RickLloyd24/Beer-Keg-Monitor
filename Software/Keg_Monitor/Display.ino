/* Display Update */
void DisplayUpdate(void) {
  //Serial.println("Entered DisplayUpdate Function");
  cv.clear();
  cv.setPenColor(Color::BrightYellow);
  cv.drawText(0, Row0,    "       Rick's Beer Keg Monitor");  
  cv.setPenColor(Color::Cyan);
  cv.drawTextFmt(0, Row1, "     Average Temperature %4.1f F", Temperature[0]);
  cv.setPenColor(Color::BrightWhite);
  cv.drawText(Col0, Row2,   "Tap Glasses       Style         Alc Days"); 
  cv.setPenColor(Color::White);
  int cRow = Row3;
  for (int j = 0; j < numscales; j++) {
    cv.setPenColor(Color::BrightWhite);cv.drawTextFmt(Col1, cRow, "%d ", j+1);
    if (GlassesLeft[j] > 30) cv.setPenColor(Color::BrightGreen);
    else if (GlassesLeft[j] < 10) cv.setPenColor(Color::BrightRed);
    else cv.setPenColor(Color::Yellow);
    cv.drawTextFmt(Col4, cRow, " %5.1f ", GlassesLeft[j]);
    cv.setPenColor(Color::Magenta);
    drawTextStr(Col12, cRow, BeerNames[j]);
    cv.setPenColor(Color::Yellow);
    cv.drawTextFmt(Col32, cRow,   "%3.1f  %3d", Alcohol[j], DaysKegged[j] );
    cRow = cRow + Row;
  }
  cv.setPenColor(Color::BrightBlue);
  cv.drawText(0, Row8, "Temp #1 "); 
  if (UsingSensor  == 1){
    cv.setPenColor(Color::Cyan); 
    cv.drawTextFmt(Col8, Row8, "%4.1f F ", Temperature[1]);
    cv.setPenColor(Color::BrightBlue);
    cv.drawTextFmt(Col16, Row8, "#2 %4.1f F", Temperature[2]);
  }
  else {
    cv.drawTextFmt(Col8, Row8, "%4.1f F ", Temperature[1]);
    cv.setPenColor(Color::Cyan);
    cv.drawTextFmt(Col16, Row8, "#2 %4.1f F ", Temperature[2]);
  }
  cv.setPenColor(Color::BrightBlue);
  cv.drawTextFmt(Col27, Row8, "Delta %4.1f F", TemperatureBias[0]);
  cv.drawTextFmt(0, Row9,   " Max %4.1f Min %4.1f ", HighTemp, LowTemp);    
  if (FreezerState == 1) {
    cv.setPenColor(Color::BrightGreen);
    cv.drawTextFmt(Col20, Row9, "Freezer On  %2d", FreezerOnPercent);  
  }
  else {
    cv.setPenColor(Color::Red);
    cv.drawTextFmt(Col20, Row9, "Freezer Off %2d", 100 - FreezerOnPercent);  
  }  
  cv.drawText(Col35, Row9, "%");  
  if (AlarmCnt == 0) {
    cv.setPenColor(Color::Green);
    cv.drawText(0, Row10, "No Alarms");
  }  
  else { 
    cv.setPenColor(Color::BrightRed);
    cv.drawTextFmt(0, Row10, "Alarms set %d ", AlarmCnt);
  }
  cv.setPenColor(Color::Magenta);
  drawTextStr(Col13, Row10, DateTime); 
  cv.setPenColor(Color::White);
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
  cv.drawTextFmt(0, Row9, "Temp i Bias ff.f (%4.1f %4.1f delta %4.1f)", TemperatureBias[1], TemperatureBias[2], TemperatureBias[0]);
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
