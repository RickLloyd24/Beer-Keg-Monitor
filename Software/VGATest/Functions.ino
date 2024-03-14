
void DisplayPrimaryColors(void) {
  cv.setBrushColor  (65, 0,  0); cv.fillRectangle(0, 0, 104, 16);
  cv.setBrushColor  (129, 0,  0); cv.fillRectangle(104, 0, 208, 16);
  cv.setBrushColor  (193, 0,  0); cv.fillRectangle(208, 0, 312, 16);
  cv.setBrushColor  (0, 65,  0); cv.fillRectangle(0, 16, 104, 32);
  cv.setBrushColor  (0, 129,  0); cv.fillRectangle(104, 16, 208, 32);
  cv.setBrushColor  (0, 193,  0); cv.fillRectangle(208, 16, 312, 32);
  cv.setBrushColor  (0, 0, 65); cv.fillRectangle(0, 32, 104, 48);
  cv.setBrushColor  (0, 0, 129); cv.fillRectangle(104, 32, 208, 48);
  cv.setBrushColor  (0, 0, 193); cv.fillRectangle(208, 32, 312, 48);
  cv.setPenColor  (255, 255, 255);
  cv.drawText(0, 0, "Dark Red");
  cv.drawText(14*8, 0, "Red");
  cv.drawText(27*8, 0, "Bright Red");
  cv.drawText(0, 16, "Dark Green");
  cv.drawText(14*8, 16, "Green");
  cv.drawText(27*8, 16, "Bright Green");
  cv.drawText(0, 32, "Dark Blue");
  cv.drawText(14*8, 32, "Blue");
  cv.drawText(27*8, 32, "Bright Blue");
  cv.setPenColor(Color::BrightYellow);
                   //   1234567890123456789012345678901234567890
  cv.drawText(0, 3*16, "You should see 3 shades of red, green ");
  cv.drawText(0, 4*16, "and blue. If you don't, make sure the");
  cv.drawText(0, 5*16, "wires are connected correctly and ");
  cv.drawText(0, 6*16, "adjust the resistor values. ");
}

void DisplayColorNames (void) {
  cv.setBrushColor(Color::Black);
  cv.clear();
  
  int Row = 0; int Col = 0; int i;
  for (i = ColorNum; i < ColorNum + 22; i = i + 2) {
    setBrushColor64(i);
    cv.fillRectangle(Col, Row, Col+40, Row+16);
    setPenColor64(brightWhite);
    if (i > 31) setPenColor64(black);
    drawTextStr(Col+15, Row, String(i));
    setPenColor64(brightWhite);
    drawTextStr(Col+48, Row, ColorName64[i]);
    Col = Col + 8*20;
    setBrushColor64(i+1);
    cv.fillRectangle(Col, Row, Col+40, Row+16);
    setPenColor64(brightWhite);
    if (i > 31) setPenColor64(black);
    drawTextStr(Col+15, Row, String(i+1));
    setPenColor64(brightWhite);
    drawTextStr(Col+48, Row, ColorName64[i+1]);
    Row = Row + 16; Col = 0;
    if (i >= 62) break;
    //Serial.println("i " + String(i) + " Col " + String(Col) + " Row " + String(Row));
  }
  ColorNum = i;
  if (ColorNum > 63) ColorNum = 0;
  setPenColor64(brightWhite);
  cv.drawText(0, 14*16,"  Send 'c' in Serial Monitor ");
}

void DisplayColorNumbers (void) {
  cv.setBrushColor(Color::Black);
  cv.clear();
  int Row = 0; int Col = 0; int num = 0;
  while (num < 64) {
    for (Col = 0; Col < 320; Col = Col + 40){
      setBrushColor64(num);
      cv.fillRectangle(Col, Row, Col+40, Row+30);
      num++;
    }
    Row = Row + 30;
  }
  Row = 7; num = 0;
  while (num < 64) {
    for (Col = 15; Col < 320; Col = Col + 40){
      setPenColor64(brightWhite);
      if (num > 31) setPenColor64(black);
      drawTextStr(Col, Row, String(num));
      num++;
    }
    Row = Row + 30;
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
void DisplayAllColors(void) {
  int num = 0;
  for (int j = 140; j < 220; j = j + 20) {
    for (int i = 0; i < 320; i = i + 20) {
      setBrushColor64 (num); 
      cv.fillRectangle(i, j, i + 20, j + 20);
      num++;
    }
  }    
}

void InitColorNames(void) {
  ColorName64[0] = "black";
  ColorName64[1] = "darkBlue";
  ColorName64[2] = "blue";
  ColorName64[3] = "brightBlue";
  ColorName64[4] = "darkGreen";
  ColorName64[5] = "darkCyan";
  ColorName64[6] = "blueLobster";
  ColorName64[7] = "nilaBlue";
  ColorName64[8] = "green";
  ColorName64[9] = "ritaGreen";
  ColorName64[10] = "cyan";
  ColorName64[11] = "protossBlue";
  ColorName64[12] = "brightGreen";
  ColorName64[13] = "cathGreen";
  ColorName64[14] = "enthuGreen";
  ColorName64[15] = "brightCyan";
  ColorName64[16] = "darkRed";
  ColorName64[17] = "cloakRed";
  ColorName64[18] = "darkPurple";
  ColorName64[19] = "purple";
  ColorName64[20] = "darkYellow";
  ColorName64[21] = "stonegrey";
  ColorName64[22] = "brandywine";
  ColorName64[23] = "neonBlue";
  ColorName64[24] = "yoshiGreen";
  ColorName64[25] = "haraGreen";
  ColorName64[26] = "emerald";
  ColorName64[27] = "JoustBlue";
  ColorName64[28] = "hyperGreen";
  ColorName64[29] = "blobGreen";
  ColorName64[30] = "hanGreen";
  ColorName64[31] = "electricBlue";
  ColorName64[32] = "red";
  ColorName64[33] = "shyGuyRed";
  ColorName64[34] = "magenta";
  ColorName64[35] = "BriteMagenta";
  ColorName64[36] = "darkOrange";
  ColorName64[37] = "brown";
  ColorName64[38] = "orchid";
  ColorName64[39] = "violet";
  ColorName64[40] = "yellow";
  ColorName64[41] = "greenBanana";
  ColorName64[42] = "alpinWhite";
  ColorName64[43] = "shyMoment";
  ColorName64[44] = "lime";
  ColorName64[45] = "iradGreen";
  ColorName64[46] = "mint";
  ColorName64[47] = "turquoise";
  ColorName64[48] = "brightRed";
  ColorName64[49] = "neon";
  ColorName64[50] = "msPac-Man";
  ColorName64[51] = "BrightMagenta";
  ColorName64[52] = "mysticRed";
  ColorName64[53] = "fluorRed";
  ColorName64[54] = "pink";
  ColorName64[55] = "ultimatePink";
  ColorName64[56] = "orange";
  ColorName64[57] = "soulside";
  ColorName64[58] = "apricot";
  ColorName64[59] = "jigglypuff";
  ColorName64[60] = "brightYellow";
  ColorName64[61] = "pilaYellow";
  ColorName64[62] = "lemon";
  ColorName64[63] = "brightWhite";
}

void drawTextStr(int X, int Y, String S) {
  int len = S.length() + 1;
  char buf[len];
  S.toCharArray(buf, len);
  for(int i = 0; i < len; i++) {
    cv.drawChar(X + i*8, Y, buf[i]);     
  }
}
