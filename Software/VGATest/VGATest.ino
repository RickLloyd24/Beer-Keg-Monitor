/* ESP32-WROOM-DA Module, Huge APP, Core 1, Core 0 */
#include <Arduino.h>
#include "fabgl.h"                //VGA Monitor and Keyboard library

#define black         0
#define darkBlue      1
#define blue          2
#define brightBlue    3
#define darkGreen     4
#define darkCyan      5
#define blueLobster   6
#define nilaBlue      7
#define green         8
#define ritaGreen     9
#define cyan          10
#define protossBlue   11
#define brightGreen   12
#define cathGreen     13
#define enthuGreen    14
#define brightCyan    15
#define darkRed       16
#define cloakRed      17
#define darkPurple    18
#define purple        19
#define darkYellow    20
#define stonegrey     21
#define brandywine    22
#define neonBlue      23
#define yoshiGreen    24
#define haraGreen     25
#define emerald       26
#define JoustBlue     27
#define hyperGreen    28
#define blobGreen     29
#define hanGreen      30
#define electricBlue  31
#define red           32
#define shyGuyRed     33
#define magenta       34
#define BriteMagenta  35
#define darkOrange    36
#define brown         37
#define orchid        38
#define violet        39
#define yellow        40
#define greenBanana   41
#define alpinWhite    42
#define shyMoment     43
#define lime          44
#define iradGreen     45
#define mint          46
#define turquoise     47
#define brightRed     48
#define neon          49
#define msPac-Man     50
#define BrightMagenta 51
#define mysticRed     52
#define fluorRed      53
#define pink          54
#define ultimatePink  55
#define orange        56
#define soulside      57
#define apricot       58
#define jigglypuff    59
#define brightYellow  60
#define pilaYellow    61
#define lemon         62
#define brightWhite   63

int ColorNum = 0;
String ColorName64[64];
 
fabgl::VGAController VGAController;
Canvas cv(&VGAController);

void setup() {
  /* Setup Serial Ports */
  Serial.begin(115200);
  delay(500);  // avoid garbage into the UART
  Serial.write("\r\n\nReset\r\n");
  Serial.println();
  
  //VGAController.begin(GPIO_NUM_14, GPIO_NUM_13, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_23, GPIO_NUM_15);
  VGAController.begin();                                             //default pins
  VGAController.setResolution(QVGA_320x240_60Hz);
  setBrushColor64(black);
  cv.selectFont(&fabgl::FONT_8x16);
  delay(500);
  cv.clear();

  InitColorNames();
  Serial.println("Displaying Primary Colors");
  DisplayPrimaryColors();
  
  setPenColor64(lemon);
  cv.drawText(0, 120, "        Displaying all 64 colors");
  Serial.println("Displaying all 64 colors");
  DisplayAllColors();
  cv.drawText(0, 14*16,"  Send 'c' in Serial Monitor ");
}

void loop() {
/* Serial 0 Interface for debugging */
  if (Serial.available() > 0) {                                          /* Is something in the serial buffer? */
    static String s0 = "";
    s0 = Serial.readString();               
    //Serial.print("Input String: "); Serial.print(s0);
    if (s0[0] == 'c') {
      DisplayColorNames();
    }
    else if (s0[0] = 'n') {
      DisplayColorNumbers(); 
    }
    s0 = "";
  }

}
