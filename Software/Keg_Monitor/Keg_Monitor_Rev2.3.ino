/* Use ESP32 WROOM Dev Module* Huge APP, Arduino Core 1, Events Core 0 */
#include <Arduino.h>
#include <TimeLib.h>              //Time Library
#include <Wire.h>                 //Library for I2C Interface
#include "mcp9808.h"              //MCP9808 Library 
#include "fabgl.h"                //VGA Monitor and Keyboard library
#include "FS.h"                   //File System Library (required for LittleFS)
#include "LittleFS.h"             //Little File System Library  
#include <HX711_Single_Clk.h>     //HX711 Library (Custom Library requires download and Install)   
#include <DHTSimple.h>            //DHT Sensor Library (Custom Library requires download and Install)
#include "Constants.h"            //Configuration, Constants and Pin Numbers

#define maxscales 5
#define Gain 128
boolean const ScalesConnected = false;           // used for debuging
boolean const printSerial = false;

/* constructors */
fabgl::VGAController VGAController;
Canvas cv(&VGAController);
fabgl::PS2Controller     PS2Controller;
HX711O Scale(numscales);                     //constructor number of devices and size of array passed
MCP9808 Sensor1(0x18);
MCP9808 Sensor2(0x19);
MCP9808 Sensor3(0x1A);
DHTSimple dht1(DHT1Pin, DHTType);
DHTSimple dht2(DHT2Pin, DHTType);
DHTSimple dht3(DHT3Pin, DHTType);

/* Filenames */
const String ConfigFN = "/config.txt";                    /* Configuration file name */ 

/* Scale Variables */
long ScaleReadings[maxscales];
long ScaleValues[maxscales];
long PrevScaleValues[maxscales];
char ScaleDisp; 
int CalibratingFlag = -1;
float CalWeight = 0;

/* File Variables */
float EmptyKW[maxscales];                               /* Empty Keg Weight */
float FullKegWeight[maxscales];                         /* Full Keg Weight */
long FullKeg[maxscales];                                /* Full Keg Quanta */
long EmptyKeg[maxscales];                               /* Empty Keg (no keg) Quanta */
String KeggedDate[maxscales];                           /* Date Kegged mm/dd/yy format */
float Alcohol[maxscales];                               /* Percent Alcohol */
String BeerNames[maxscales];                            /* Titles for each Tap */ 
int DaysKegged[] = {0, 0, 0, 0, 0};                     /* Number of days the beer has been in the keg */
int Tempgoal = 34;                                      /* Desired Freezer Temperature */
int MaxTemp;                                            /* Maximum temp that is possible */
int MinTemp;                                            /* Minimum temp that is possible */
float HighTemp;                                         /* Maximum temperature observed */
float LowTemp;                                          /* Lowest temperature observed */

/* Temperature and Freezer Global Variables */
float Temperature[] = {0, 0, 0, 0};           /* Average Temperature, Sensor #1, Sensor #2, Sensor #3 */
float TempBias[4];                             /* Delta (#1-#2), Sensor #1 Offset, Sensor #2 Offset, Sensor #3 Offset */
int numTS = 0;
int UsingSensor = 1;

unsigned long Tensecond;
unsigned long OneMinuteTT;
unsigned long Threesecond;

String DateTime = "";
boolean TimeSet = false;
int CurrentTap = 1;
float GlassesLeft[] = {0.0, 5.0, 10.0, 25.0, 35.0};
boolean TempAvailFlag = true;
boolean FreezerState = false;
int FreezerOnPercent = 0;
String InputError = "";
String InputResults = "";
String AlarmStr = "";
int AlarmCnt = 0;
long AlarmFlag = 0;
int ScaleClk = ScaleClkPin;
int ScaleOut[] = {ScaleOut0Pin, ScaleOut1Pin, ScaleOut2Pin, ScaleOut3Pin, ScaleOut4Pin};

/* ---------------------------  Setup --------------------------------------------- */
void setup()
{
/* Setup VGA */  
  PS2Controller.begin(PS2Preset::KeyboardPort0);
  //VGAController.begin(red1Pin, red0Pin, green1Pin, green0Pin, blue1Pin, blue0Pin, HSyncPin, VSyncPin);   
  //VGAController.begin(GPIO_NUM_14, GPIO_NUM_13, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_23, GPIO_NUM_15);
  VGAController.begin();
  VGAController.setResolution(QVGA_320x240_60Hz);
  delay(500);
  cv.setPenColor(Color::Yellow);
  cv.setBrushColor(Color::Black);
  cv.selectFont(&fabgl::FONT_8x16);
  
/* Setup Serial Ports */
  Serial.begin(115200);
  delay(500);  // avoid garbage into the UART
  Serial.write("\r\n\nReset\r\n");
  Serial.println();
  Serial2.begin(9600);  
  
  delay(1000);
  const char myVer[] = __DATE__ " @ " __TIME__;
  DisplayPrint("Build: " + String(myVer));
  DisplayPrint("VGA Setup Complete");
  DisplayPrint("Serial Ports Ready");

/* Setup Pin Modes */
  pinMode(FrigControlPin, OUTPUT);
  digitalWrite(FrigControlPin, LOW);
  if (DHTPower) {
    if (DHTSensors > 0){
      pinMode(DHT1Power, OUTPUT);
      digitalWrite(DHT1Power, HIGH);
    }
    if (DHTSensors > 1) {
      pinMode(DHT2Power, OUTPUT);
      digitalWrite(DHT2Power, HIGH);
    }
    if (DHTSensors > 2) {
      pinMode(DHT3Power, OUTPUT);
      digitalWrite(DHT3Power, HIGH);
    }
  }
  DisplayPrint("Hardware Pins Set");

/* Intialize and Start Scales */
  if (ScalesConnected) {
    Scale.begin(ScaleClk, ScaleOut, Gain);
    DisplayPrint("Scales Running");
  }  
  else {
    DisplayPrint("Scales not Connected");
  }

/* Get Initial Temperature readings */ 
  int GoodTS = InitTempSensors();
  if (GoodTS == 0) {
    DisplayPrint("No good Temperature Sensors!");
  }
  else  {
    DisplayPrint(String(GoodTS) + " out of " + String(numTS) + " Temperature Sensors are good");
  }
  
/* Start LittleFS */
  if(!LittleFS.begin()){                                                         /* Is File System OK? */ 
    Serial.println("An Error has occurred while mounting LittleFS");
    DisplayPrint("File system is corrupted");                                    /* Set Critical Status */
    delay (1000);
    while(1);
  }
/* Get stored data from Config File */  
  else {
    listDir(LittleFS, "/", 1);                                                   /* List Files in FS */
    if (ReadConfigData()) {                                                      /* Read Configuration data from file */
      Serial.println("Displaying Configuration Variables");
      PrintConfigData();                                                         /* display data read */
      DisplayPrint("Config Data Good");                                        /* Set Good Status */
    }
    else {                                                                       /* Configuration data corrupted */
      DisplayPrint("Config Data read error");                                         /* Set Critical Status */
      delay(1000);
      while(1);
    }
  }
  DisplayPrint("Setup Complete");
  delay(10000);

/* Stagger Tasks times to reduce CPU load  */
  Tensecond = millis() + TENSECONDMILS + 200;
  Threesecond = millis() + THREESECONDMILS + 1300;
  OneMinuteTT = millis() + MINUTEMILS + 2500;
  
  cv.clear();
  DisplayUpdate();

}
/* ---------------------------  Main Loop  --------------------------------------------- */
unsigned long curtime = 0;                             /* Curerent running time in millis */

void loop()  {
  curtime = millis();

/* Serial 0 Interface for debugging */
  if (Serial.available() > 0) {                                          /* Is something in the serial buffer? */
    static String s0 = "";
    s0 = Serial.readString();               
    Serial.print("Input String: "); Serial.print(s0);
    ProcessCommand(s0);
    s0 = "";
  }
/* Check if information received on Port 2 */
  if (Serial2.available() > 6) {                    /* Check if anything is in the buffer */
    static String s2;
    String s = Serial2.readString();                 /* Read data in serial port */
    //s = s + s2;                                      /* Add any left over chars */
    int x = s.length();
    if (printSerial) Serial.println("Rcvd " + String(x) + " chars: " + s);       /* Print that something was Received */
    int start; int last; int i;
    for (i = 0; i < x; i++){                     /* Search until a command or the end of the string */
      if (s.charAt(i) == '!') {   
        start = i;      
      }
      if (s.charAt(i) == ';') {                      /* End of command */
        last = i;                                    /* Save end of cmd ptr */
        if ((last - start) > 6) {
          ProcessSerial2(s.substring(start, last));        /* Process the command */
        }
      }
    }
    s2 = s.substring(last+1, i);
    if (printSerial) Serial.println("Left Over " + String(s2.length()) + s2);
  }    
  
/* Check if a key has been pressed */
  auto keyboard = PS2Controller.keyboard();
  if (keyboard->virtualKeyAvailable()) {
    VirtualKeyItem item;
    if (keyboard->getNextVirtualKey(&item)) {
      char key = item.ASCII;
      if(item.down == 0) {                                                 /* Key Released */
        //Serial.print(key); Serial.println(" key Pressed");
        ProcessKey(key);
      }
    }
  }      
/* 3 Second Tasks */
  if (curtime > Threesecond) {                                                     
    Threesecond = Threesecond + THREESECONDMILS;
    if (CalibratingFlag != -1) {
        if (ScalesConnected) {
          ProcessScaleValues();
        }  
      }
    }


/* 10 Second Tasks */  
  if (curtime > Tensecond) {                                                     
    Tensecond = Tensecond + TENSECONDMILS;
    GetTemperature();
    if (ScalesConnected  && CalibratingFlag == -1) {
      ProcessScaleValues();
    }  
  }  
/* 1 Minute Tasks */  
  if (curtime > OneMinuteTT) {         
    OneMinuteTT = OneMinuteTT + MINUTEMILS;
    SendGlasses(); 
    SendDaysKegged();
    SendAlcohol();
    SendStyle();
    SendAlarms();
    if (TimeSet) DateTime = DateTimeStr();
    if(DisplayMode == Normal) DisplayUpdate();
    SendTemperature();
  }
}
