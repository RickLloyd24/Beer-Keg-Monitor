/* Temperature Sensor Configuration */
/* You must have at least one sensor, a maximum of 2 of each type of sensor are allowed */
#define MCPSensors 2          //Number of MCP9808 Sensors SCLpin and SDApin must have valid pin numbers
#define DS18B20Sensors 0      //Number of DS18B20 Sensors ONE_WIRE_BUS must have a valid pin number
#define DHTSensors 0          //Number of DHT Sensors 
#define DHTType 2             //DHT11 is 1, All others 2 (DHT21, DHT22, AM2301, AM2302) 
#define DHT1Pin -1            //Pin must be changed if DHT sensor is used 
#define DHT2Pin -1            //Pin must be changed if DHT sensor is used
#define DHT3Pin -1            //Pin must be changed if DHT sensor is used
#define ONE_WIRE_BUS -1       //Pin must be changed if DS18B20 Sensors are used

#define CO2OnlyFlag false
#define numscales 5                      /* Number of Scales and taps */
#define numColors 64                     /* Number of display colors 8 or 64.  8 usese 5 wires and 64 uses 8 wires */

/* Pin assignments */
#define CO2ValvePin 0                   // CO2 Solenoid On/Off
#define TXD0 1                          //UART 0 TX, Debug output at boot
#define ScalePowerPin 2
#define RXD0 3                          //UART 0 RX, High at boot, UART 0 Rx
#define Blue0Pin 4
#define Blue1Pin 5
#define ScaleOut0Pin 12                  //boot fails if pulled high
#define SCLpin 13                        
#define SDApin 14                        //outputs PWM signal at boot  
#define VSyncPin 15                      //outputs PWM signal at boot 
#define SecondaryOnOffPin 16             //UART 2 RX
#define PrimaryOnOffPin 17               //UART 2 TX
#define Green0Pin 18
#define Green1Pin 19
#define Red0Pin 21                        //Default SDA
#define Red1Pin 22                        //Default SCL
#define HSyncPin 23
#define ScaleOut4Pin 25              
#define PrimarySecondaryPin 26
#define ScaleClkPin 27
#define KeyboardDataPin 32
#define KeyboardClkPin 33
#define ScaleOut3Pin 34
#define CO2PressurePin 35                //Receive Only
#define ScaleOut1Pin 36                   //Receive Only VP
#define ScaleOut2Pin 39                   //Receive Only VN

/* Time Variables */
const char* ntpServer = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -480*60;                       // Los Angeles Time Zone -8 hours
const int   daylightOffset_sec = 3600;                     // Set to 0 if you don't have DST
unsigned long MidNightReset = 0;
String DateTime = "";
boolean TimeSet = false;

/* Scale Variables */
#define maxscales 5
#define Gain 128
long ScaleReadings[maxscales];
long ScaleValues[maxscales];
long PrevScaleValues[maxscales];
char ScaleDisp; 
float CalWeight = 0;
unsigned long ScaleOnTime = 0;
boolean ScaleOnFlag = false;

/* WiFi Variables */
String IPaddr = "";                                     /* IP Address */ 
float RSSIlvl = 31.0;                                   /* Current RSSI level in dBm */
boolean WiFiflag = false;                               /* Flag if WiFi is connected */
const char* ssid     = "Network_Name";                     //Wi-Fi network Name
const char* password = "PASSWORD";                      //Password to log on to your internal Wi-Fi network  

/* Common global defines */
#define On 1
#define Off 0
#define MINUTES3 1000*60*3
#define MINUTES10 1000*60*10
#define MINUTES30 1000*60*30
#define DAYSECONDS 3600*24
#define THREESECONDMILS 1000*3
#define TENSECONDMILS 1000*10
#define MINUTEMILS 1000*60
#define HOURMILS 1000*3600
#define EpochYear 2023
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321
#define EpochUnix 1704096000  //Unix Time Jan 1, 2023 00:00
#define TimeZoneOffset -8  //Standard Time Zone Offset Hours  Los Angeles is 8 hours
#define EpochYear 2024
#define MaxDeltaTemp 3
#define MaxStableCnt 9
#define Primary LOW
#define Secondary HIGH
#define TEMPERATURE_PRECISION 9

/* Display Variables */
#define Command 1
#define Alarm 2
#define Scales 3
#define Normal 4
#define Plot 5
#define ScreenSaverStart HOURMILS                  //2*HOURMILS
unsigned long ScreenSaverStartTime = 0;
boolean ScreenSaverOn = false;
int DisplayMode = Normal;

/* Alarm Flag bits */
#define Alarm0 0  //Serial2: Invalid Scale number 
#define Alarm1 1  //Scale TimeOut
#define Alarm2 2  //Scale Not Connected 
#define Alarm3 3  //Available
#define Alarm4 4  //Temp Sensors don't match
#define Alarm5 5  //Temp Sensor Unreasonable
#define Alarm6 6  //MCP Sensor Read Error
#define Alarm7 7  //All Temp Sensors Bad
#define Alarm8 8  //DHT Temp Sensor Read Error
#define Alarm9 9  //Reset HX711 
#define Alarm10 10  //Freezer too cold switched to Secondary 
#define Alarm11 11  //Freezer too hot switched to Secondary 
#define Alarm12 12  //DS18B20 Temperature Read Error
#define Alarm13 13  //CO2 Reading below Minimum PSI
#define Alarm14 14  //CO2 Tank Pressure is Low, Replace Tank
#define Alarm15 15  //CO2 Reading below 100 PSI, Leak

/* Key Definitions */
#define Esc 27
#define CarriageReturn 13
#define Backspace 8
#define Space 32

/* Color Definitions 64 colors */
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
#define alpineWhite   42
#define shyMoment     43
#define lime          44
#define iradGreen     45
#define mint          46
#define turquoise     47
#define brightRed     48
#define neon          49
#define msPacMan      50
#define brightMagenta 51
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
#define white         63

/* Color Definitions 8 colors */
#define Bla black
#define Blu brightBlue
#define Gre brightGreen
#define Cya brightCyan
#define Red brightRed
#define Mag brightMagenta
#define Yel brightYellow
#define Whi white

/* Display Variables */
#define MaxRows 15
#define StyleLen 20
#define Row 16
#define Row0 Row*0
#define Row1 Row*1
#define Row2 Row*2
#define Row3 Row*3
#define Row4 Row*4
#define Row5 Row*5
#define Row6 Row*6
#define Row7 Row*7
#define Row8 Row*8
#define Row9 Row*9
#define Row10 Row*10
#define Row11 Row*11
#define Row12 Row*12
#define Row13 Row*13
#define Row14 Row*14
#define Row15 Row*15
#define Col 8
#define Col0 Col*0
#define Col1 Col*1
#define Col2 Col*2
#define Col3 Col*3
#define Col4 Col*4
#define Col5 Col*5
#define Col6 Col*6
#define Col7 Col*7
#define Col8 Col*8
#define Col9 Col*9
#define Col10 Col*10
#define Col11 Col*11
#define Col12 Col*12
#define Col13 Col*13
#define Col14 Col*14
#define Col15 Col*15
#define Col16 Col*16
#define Col17 Col*17
#define Col18 Col*18
#define Col19 Col*19
#define Col20 Col*20
#define Col21 Col*21
#define Col22 Col*22
#define Col23 Col*23
#define Col24 Col*24
#define Col25 Col*25
#define Col26 Col*26
#define Col27 Col*27
#define Col28 Col*28
#define Col29 Col*29
#define Col30 Col*30
#define Col31 Col*31
#define Col32 Col*32
#define Col33 Col*33
#define Col34 Col*34
#define Col35 Col*35
#define Col36 Col*36
#define Col37 Col*37
#define Col38 Col*38
#define Col39 Col*39
