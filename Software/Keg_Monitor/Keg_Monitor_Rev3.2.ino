/* Use ESP32 WROOM Dev Module* Huge APP, Arduino Core 1, Events Core 0 
   Set Serial Monitor to 9600 Baud */
#include <Arduino.h>
#include <WiFi.h>                 //Wi-Fi Library
#include <TimeLib.h>              //Time Library
#include <Wire.h>                 //Library for I2C Interface
#include "mcp9808.h"              //MCP9808 Library 
#include "fabgl.h"                //VGA Monitor and Keyboard library
#include "FS.h"                   //File System Library (required for LittleFS)
#include "LittleFS.h"             //Little File System Library  
#include <HX711_Single_Clk.h>     //HX711 Library (Custom Library requires download and Install)   
#include <DHTSimple.h>            //DHT Sensor Library (Custom Library requires download and Install)
#include <OneWire.h>              //Arduino Library for One Wire devices
#include <DallasTemperature.h>    //DS18B20 device library "Arduino-Temperature-Control-Library"
#include "Constants.h"            //Configuration, Constants and Pin Numbers

boolean ScalesConnected = true;                 // used for debuging
boolean const printSerial = false;
int ptr = 0;

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
OneWire oneWire(ONE_WIRE_BUS);                               // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);                         // Pass our oneWire reference to Dallas Temperature
DeviceAddress DS18B20Temp1, DS18B20Temp2, DS18B20Temp3;      // arrays to hold device addresses

/* Filenames */
const String ConfigFN = "/config.txt";                    /* Configuration file name */ 

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
unsigned long curtime = 0;                             /* Current running time in millis */

/* CO2 Variables */
#define CO2BufLen 290             //Number of samples to Plot
long CO2Pressure = 0;             //current pressure psi
long CO2Rate = 60;                //On Rate in minutes
unsigned long CO2OnTime = 30000;  //CO2 Next On Time
unsigned long CO2Reset = 0;       //Reset Rate back to 60 minutes
int MaxPressReading = 0;          //CO2 Maximum Pressure Reading
int MinPressReading = 0;          //CO2 Minimum Pressure Reading
int CO2BufPtr = 0; 
int CO2PressureBuf[CO2BufLen];
int CO2PressureChg = 0;
float offset = 0; float prevOffset = 0;
float slope = 0;  float prevSlope = 0;

int CurrentTap = 1;
float GlassesLeft[] = {5.0, 10.0, 15.0, 20.0, 30.0};
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
String DisplayUpdateStr = "";

/* ---------------------------  Setup --------------------------------------------- */
void setup()
{
/* Setup VGA */  
  PS2Controller.begin(PS2Preset::KeyboardPort0);
  //VGAController.begin(GPIO_NUM_22, GPIO_NUM_19, GPIO_NUM_5, GPIO_NUM_23, GPIO_NUM_15);
  // Use GPIO 22-21 for red, GPIO 19-18 for green, GPIO 5-4 for blue, GPIO 23 for HSync and GPIO 15 for VSync
  //VGAController.begin(GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_23, GPIO_NUM_15);
  VGAController.begin();
  VGAController.setResolution(QVGA_320x240_60Hz);
  delay(500);
  cv.setPenColor(Color::Yellow);
  cv.setBrushColor(Color::Black);
  cv.selectFont(&fabgl::FONT_8x16);

  Serial.begin(115200);
  delay(500);  // avoid garbage into the UART

  const char myVer[] = __DATE__ " @ " __TIME__;
  DisplayPrint("Build: " + String(myVer));
  DisplayPrint("VGA Setup Complete");

  pinMode(CO2ValvePin, OUTPUT);
  digitalWrite(CO2ValvePin, LOW);
  if (CO2OnlyFlag) CO2OnlyLoop();

/* Intialize WiFi */
 if (InitWiFi()) {
    DisplayPrint("Connected to WiFi! " + String(RSSIlvl,1) + " dBm");
  }  
  else {
    DisplayPrint("Failed to Connected to WiFi!");
  }

/* Setup Pin Modes */
  pinMode(PrimaryOnOffPin, OUTPUT);
  pinMode(SecondaryOnOffPin, OUTPUT);
  pinMode(PrimarySecondaryPin, OUTPUT);
  digitalWrite(PrimarySecondaryPin, Primary);
  digitalWrite(PrimaryOnOffPin, LOW);
  digitalWrite(SecondaryOnOffPin, LOW);
  pinMode(ScalePowerPin, OUTPUT);
  digitalWrite(ScalePowerPin, HIGH);
  ScaleOnFlag = true;
  DisplayPrint("Hardware Pins Set");

/* Set Time from NTP Service */
  DisplayPrint("Getting Current Time");
  delay(1000);
  if (!SetRTCTime()) {
    DisplayPrint("Failed to obtain Time");
    DateTime = "No Time Available";
    MidNightReset = DAYSECONDS;
  }
  else {
    TimeSet = true;
    DateTime = DateTimeStr();
    DisplayPrint("Current Time is " + DateTime);
    MidNightReset = (DAYSECONDS - (hour()*3600 + minute()*60) + second()) * 1000 - millis();
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
  /* Intialize and Start Scales */
  if (ScalesConnected) {
    InitializeScales();
  }  
  else {
    DisplayPrint("Scales not Connected");
  }

  DisplayPrint("Setup Complete");
  delay(5000);

  cv.clear();
  ClearDisplayUpdateStr();
  DisplayUpdate();

/* Stagger Tasks times to reduce CPU load  */
  curtime = millis();
  Tensecond = curtime + TENSECONDMILS + 200;
  OneMinuteTT = curtime + MINUTEMILS + 2500;
  CO2OnTime = curtime + TENSECONDMILS*3 + 600;
  ScreenSaverStartTime = curtime + ScreenSaverStart;
}
/* ---------------------------  Main Loop  --------------------------------------------- */
void loop()  {
  curtime = millis();

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

/* 10 Second Tasks */  
  if (curtime > Tensecond) {                                                     
    Tensecond = Tensecond + TENSECONDMILS;
    GetTemperature();
    if (ScalesConnected) ProcessScaleValues();
    ReadCO2Pressure();
  }  
/* 1 Minute Tasks */  
  if (curtime > OneMinuteTT) {         
    OneMinuteTT = OneMinuteTT + MINUTEMILS;
    if (curtime > ScreenSaverStartTime) {
      if (ScreenSaverOn == false) {
        cv.clear();
        ScreenSaverOn = true;
        Serial.println("Screen Saver is On");
      }  
    }
    if (TimeSet) DateTime = DateTimeStr();
    CalcDaysKegged(DateTime);
    if(DisplayMode == Normal) DisplayUpdate();
    if(curtime > MidNightReset) ESP.restart();
  }
/* CO2 Control Tasks */  
  if (curtime > CO2OnTime) {         
    CheckCO2();
  }
/* Check if Scale is Ready */  
  if (ScaleOnFlag == false) {
    if (curtime > ScaleOnTime) {
      if (digitalRead(ScalePowerPin) == HIGH) {
        ScaleOnFlag = true;  
      }
      else {
        digitalWrite(ScalePowerPin, HIGH);
        ScaleOnTime = curtime + 1000;                            //Scale not ready for 450 ms to set flag to true
      }
      
    }
  }
}
