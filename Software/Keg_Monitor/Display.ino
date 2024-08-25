/* Display Update */
void DisplayUpdate(void) {
  if (ScreenSaverOn) return;
  int color;
  drawText(0, Row0, lemon,"       Rick's Beer Keg Monitor");  
  drawTextFmt(Col5, Row1, electricBlue, "Average Temperature", turquoise, "%5.1f", haraGreen, " F", Temperature[0]);
  drawText(Col0, Row2, white, "Tap Glasses       Style         Alc Days"); 
  int cRow = Row3;
  for (int j = 0; j < numscales; j++) {
    drawText(Col1, cRow, white, String(j+1));
    if (GlassesLeft[j] > 20) {
      drawFmt(Col4, cRow, brightGreen, "%5.1f ", GlassesLeft[j]);
    }
    else if (GlassesLeft[j] > 10) {
      drawFmt(Col4, cRow, brightYellow, "%5.1f ", GlassesLeft[j]);
    }
    else {
      drawFmt(Col4, cRow, brightRed, "%5.1f ", GlassesLeft[j]);
    }
    drawText(Col12, cRow, orange, BeerNames[j]);
    drawFmt(Col32, cRow, turquoise, "%3.1f", Alcohol[j]);
    drawFmt(Col37, cRow, greenBanana, "%3.0f", DaysKegged[j]);
    cRow = cRow + Row;
  }
  /* Temperature Sensors */
  drawText(Col0, Row8, neonBlue, "Temp");
  if (numTS == 0) {
    cv.drawText(0, Row8, BriteMagenta, "No Temperature Sensors Installed"); 
  }
  else if (numTS == 1) {
    drawTextFmt(Col5, Row8, Whi, "#1 ", Whi, "%4.1f", Gre, " F", Temperature[1]);
  }
  else if (numTS == 2) {
    if (UsingSensor == 1) {
      drawTextFmt(Col5, Row8, Whi, "#1 ", Whi, "%4.1f", Gre, " F", Temperature[1]);
      drawTextFmt(Col16, Row8, Blu, "#2 ", Blu, "%4.1f", Gre, " F", Temperature[2]);
    }
    else {
      drawTextFmt(Col8, Row8, Blu, "#1 ", Blu, "%4.1f", Gre, " F", Temperature[1]);
      drawTextFmt(Col16, Row8, Whi, "#2 ", Whi, "%4.1f", Gre, " F", Temperature[2]);  
    }
    drawTextFmt(Col27, Row8, haraGreen, "Delta ", Cya, "%4.1f", yoshiGreen, " F", Temperature[1] - Temperature[2]);
  }
  else {
    int Ucolor[] = {Blu, Blu, Blu, Blu};
    Ucolor[UsingSensor] = Whi;
    drawTextFmt(Col5, Row8, Ucolor[1], "#1", Ucolor[1], "%5.1f", haraGreen, " F", Temperature[1]);
    drawTextFmt(Col16, Row8, Ucolor[2], "#2", Ucolor[2], "%5.1f", haraGreen, " F", Temperature[2]);
    drawTextFmt(Col27, Row8, Ucolor[3], "#3", Ucolor[3], "%5.1f", haraGreen, " F", Temperature[3]);
  }
  drawTextFmt(Col0, Row9, neonBlue, "Max", Yel, "%5.1f", yoshiGreen, " F", HighTemp);
  drawTextFmt(Col11, Row9, neonBlue, "Min", Yel, "%5.1f", yoshiGreen, " F", LowTemp);
  if (digitalRead(PrimaryOnOffPin) == 1  || digitalRead(SecondaryOnOffPin) == 1) {
    drawTextFmt(Col22, Row9, Mag, "Freezer On ", Yel, "%3.0f", Blu, "", FreezerOnPercent);
  }
  else {
    drawTextFmt(Col22, Row9, Blu, "Freezer Off", Yel, "%3.0f", Blu, "", 100 - FreezerOnPercent);
  }
  drawText(Col36, Row9, Gre, "%");
  if(digitalRead(PrimarySecondaryPin) == Primary) {
    drawText(Col38, Row9, Blu, "P"); 
  }
  else {
    drawText(Col38, Row9, neon, "S");
  }
  if (AlarmCnt == 0) {
    drawText(0, Row10, Gre, "No Alarms");
  }  
  else {
    drawText(0, Row10, Red, "Alarms set " + String(AlarmCnt));
  }
  drawText(Col13, Row10, brown, DateTime); 
  int sn = CurrentTap - 1;
  drawText(Col0, Row11, stonegrey, "Tap " + String(CurrentTap) + "   Full " +  String(FullKeg[sn]) + " Weight " + String(FullKegWeight[sn],1) + " lbs");  
  long delta = ScaleValues[sn]- PrevScaleValues[sn];
  float deltaoz = delta / (abs(FullKeg[sn] - EmptyKeg[sn])/(FullKegWeight[sn]*16));
  if (ScaleOnFlag) {
    drawText(Col0, Row12, stonegrey, "On ");
  }
  else {
    drawText(Col0, Row12, stonegrey, "Off ");
  }
  drawText(Col5, Row12, stonegrey, "Current " + String(ScaleReadings[sn]));
  drawTextFmt(Col21, Row12, stonegrey, "Delta", stonegrey, "%9.2f",  stonegrey, " oz", deltaoz);  
  drawText(Col7, Row13, stonegrey, "Empty " + String(EmptyKeg[sn]) + " Weight " + String(EmptyKW[sn], 1) + " lbs");  
  drawTextFmt(Col0, Row14, JoustBlue, "CO2", brown, "%5.0f", blobGreen, " psi", CO2Pressure);
  drawTextFmt(Col13, Row14, JoustBlue, "Chg", brown, "%5.0f", blobGreen, " psi", (CO2PressureChg));
  if (CO2Rate == 99) {
    drawText(Col27, Row14, mysticRed, "Valve Off    ");
  }
  else if (CO2Rate == 0) {
    drawText(Col27, Row14, fluorRed, "Valve On      ");
  }
  else {
    drawTextFmt(Col27, Row14, JoustBlue, "On in", brown, "%4.0f", blobGreen, " Min", (CO2OnTime - millis())/60000);
  }
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
  cv.drawTextFmt(0, Row5, "Rate ii min, CO2 Rate is %3d", CO2Rate);
  cv.drawText(0, Row6, "Set (Toggle Secondary/Primary Relay)");
  cv.drawText(0, Row7, "Save to file");
  cv.drawText(0, Row8, "Date mm/dd/yy hh:mm");
  cv.drawTextFmt(0, Row9, "Temp i Bias ff.f (%4.1f, %4.1f, %4.1f)", TempBias[1], TempBias[2], TempBias[3]);
  cv.drawTextFmt(0, Row10, "Goal xx (Current %2d) degrees", Tempgoal);
  // Row 11 Status
  // Row 12 Error
  if (InputError != "") {
    Serial.println(InputError);
    cv.setPenColor(Color::BrightRed);
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
void DisplayCalStatus(String s) {
  ClearRow(Row13);
  cv.setPenColor(Color::Green);
  drawTextStr(0, Row13, s);
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
    cv.setPenColor(Color::BrightRed);
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
          cv.drawTextFmt(j*Col*8, curRow, "%7d", ScaleReadings[j]);
        }
        if(curRow < MaxRows*Row) ClearRow(curRow + Row);
        break;
      case 'd':                                                                  /* Delta Outputs */
        static unsigned long PrevScaleReadings[] = {0, 0, 0, 0, 0};
        ClearRow(curRow);
        for (int j = 0; j<numscales; j++){
          if (PrevScaleReadings != 0) {
            cv.drawTextFmt(j*Col*8, curRow, "%7d", (ScaleReadings[j]- PrevScaleReadings[j]));
          }
          PrevScaleReadings[j] = ScaleReadings[j];
        }  
        if(curRow < MaxRows*Row) ClearRow(curRow + Row);
        break;
      case 'h':
        DisplayScaleCommands();                                                        
        break;
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
    //Serial.println(a);
    bitSet(AlarmFlag, alarmNum);
    AlarmCnt++;
    if (AlarmCnt < (MaxRows - 2)) {
      //AlarmStr = AlarmStr + String(alarmNum) + " " + a + ";";
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

void ClearDisplayUpdateStr(void) {
  cv.clear();
                                       //012345678901234567890123456789012345678901234567890123456789
  DisplayUpdateStr =                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //0
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //1
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //2
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //3
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //4
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //5
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //6
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //7
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //8
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //9
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //10
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //11
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //12
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //13
  DisplayUpdateStr = DisplayUpdateStr + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    //14
}
  
void drawText(int col, int row, int color, String s) {
  int first = row/16*40 + col/Col;
  int last = first + s.length();
  String myString = DisplayUpdateStr.substring(first, last);
  if (myString.equals(s)) {
    //Serial.println("No Display change");
  }
  else {
    cv.setBrushColor(Color::Black);
    cv.fillRectangle(col, row, col + s.length()*Col, row + Row);
    setPenColor64(color);
    drawTextStr(col, row, s);
    for (int i = first; i < last; i++) {
      DisplayUpdateStr[i] = s[i - first];
     }
    //Serial.println("Display " + String(row/Row) + ", " + String(col/Col) + " New Value " + s + " Old " + myString);
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

void drawTextFmt (int col, int row, int descColor, String desc, int varColor, String varfmt, int unitsColor, String units, float var) {
  String varStr = Sprintf(varfmt, var);
  setPenColor64(descColor);
  drawText(col, row, descColor, desc);  
  setPenColor64(varColor);
  drawText(col + (desc.length()* Col), row, varColor, varStr);
  setPenColor64(unitsColor);
  drawText(col + (desc.length() + varStr.length())* Col, row, unitsColor, units);
}

void drawFmt (int col, int row, int varColor, String varfmt, float var) {
  String varStr = Sprintf(varfmt, var);
  drawText(col, row, varColor, varStr);
}


/* Arduino does not support sprintf with floats 
 * This function simulates the sprinf for floats 
 * * format i.e. %6.2f - x34.12, %5.0f  - x1234
 */
String Sprintf(String format, float value) {
  char str[10];                                                    // Allocate a character array to store the result

  if (format.charAt(0) != '%') return "Err1";
  int field = format.charAt(1) - '0';
  if (format.charAt(2) != '.') return "Err2";
  int numdecimal = format.charAt(3) - '0';
  if (format.charAt(4) != 'f') return "Err3";
  if (numdecimal == 0) {
    //Serial.println("numdecimal " + String(value));
    return frstr(String(int(value)), field);
  }
  else {
    dtostrf(value, field, numdecimal, str);
  }  
  return String(str);
}

/* Fixed String Length leading blanks right justified */
String frstr(String v, int len) {
  String blank = "          ";
  if (v.length() > len) {                                            /* check if string is too long */
    Serial.println("String is too long " + v + " length " + String(len));
    v = v.substring(0, len - 1);                                     /* cut off end of string */
    return v;
  }  
  v = blank.substring(0, len - v.length()) + v;                     /* Pad to len chars */
  return v;
}

void DisplayPlot ( void) {
  #define x0 3*Col+5                    //x-axis start
  #define y0 2*Row+5                    //y-axis start
  #define ymid y0+(240-y0)/2            //y-axis midpoint
  MaxPressReading = 0; 
  MinPressReading = MaxPressure; 
  for (int i = 0; i < CO2BufPtr - 1; i++) {
    if (CO2PressureBuf[i] > MaxPressReading) MaxPressReading = CO2PressureBuf[i];
    if (CO2PressureBuf[i] < MinPressReading) MinPressReading = CO2PressureBuf[i];
  }
  MaxPressReading = MaxPressReading + 50;
  Serial.println("Display Plot") ;
  cv.clear();
  ClearDisplayUpdateStr();
  setPenColor64(Whi);
  drawTextStr(Col0, Row2, String(MaxPressReading));
  drawTextStr(Col0,     Row13, "  0");
  drawTextStr(Col0 + 5, Row14, "  0");
  setPenColor64(JoustBlue);
  cv.drawLine(x0, ymid, x0+CO2BufLen, ymid);                // X-axis
  cv.drawLine(x0, y0, x0, 240 - Row - 5);                   // Y-axis
  setPenColor64(Whi);
  float PixelsPerPSI = 240.0 / float(MaxPressReading);
  int y1; int y2;
/* Plot Raw Date */  
  for (int i = 1; i < CO2BufPtr - 1; i++) {
    y1 = (MaxPressReading - CO2PressureBuf[i-1])*PixelsPerPSI;
    y2 = (MaxPressReading - CO2PressureBuf[i])*PixelsPerPSI;
    cv.drawLine(i - 1 + x0, y1 + y0, i + x0, y2 + y0);
  }
/* Plot Linear Regression Data */
  setPenColor64(Gre);
  if (CO2BufPtr > MinPoints) {
    LinearRegression();
    y2 = (MaxPressReading - offset) * PixelsPerPSI;
    Serial.println("Offset = " + String(int(offset)) + ", y2 = " + String(y2));
    drawTextStr(Col0, y2 + y0, String(int(offset)));
    y1 = (MaxPressReading - (offset + slope*1))*PixelsPerPSI;   
    y2 = (MaxPressReading - (offset + slope*CO2BufPtr))*PixelsPerPSI; 
    cv.drawLine(x0, y1 + y0, (CO2BufPtr - 1) + x0, y2 + y0);
    int maxX = x0 + CO2BufPtr; if (maxX > Col37) maxX = Col37;
    drawTextStr(maxX, Row14, String(CO2BufPtr));
    drawTextFmt(Col0, Row0, JoustBlue, "Mean", Gre, "%4.0f", turquoise, " psi", int(offset));
    drawTextFmt(Col13, Row0, JoustBlue, "Slope", Gre, "%9.4f", turquoise, "", slope);
    drawTextFmt(Col28, Row0, JoustBlue, "Rate", brown, "%4.0f", turquoise, " Min", CO2Rate);
  }  
  else { 
    drawTextStr(Col0, Row0, "Waiting for measurements, " + String(CO2BufPtr) + " points");
    Serial.println("Waiting for measurements, ");
  }  
  drawTextFmt(Col0, Row1, JoustBlue, "Prev", brown, "%4.0f", turquoise, " psi", prevOffset);
  drawTextFmt(Col13, Row1, JoustBlue, "Slope", brown, "%9.4f", turquoise, "", prevSlope);
  if (CO2Rate == 99) {
    drawText(Col28, Row1, mysticRed, "Valve Off    ");
  }
  else if (CO2Rate == 0) {
    drawText(Col28, Row1, fluorRed, "Valve On      ");
  }
  else {
    drawTextFmt(Col28, Row1, JoustBlue, "On in", brown, "%3.0f", turquoise, " Min", (CO2OnTime - millis())/60000);
  }
  if (AlarmCnt == 0) {
    drawText(Col5, Row13, Gre, "No Alarms");
  }  
  else {
    drawText(Col5, Row13, Red, "Alarms set " + String(AlarmCnt));
  }
  
}
