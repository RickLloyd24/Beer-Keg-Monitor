/* Temperature Sensor Configuration */
/* You must have at least one sensor, a maximum of 2 of each type of sensor are allowed */
#define MCPSensors 2          //Number of MCP9808 Sensors
#define DHTSensors 1          //Number of DHT Sensors
#define DHTType 2             //DHT11 is 1, All others 2 (DHT21, DHT22, AM2301, AM2302) 
#define DHTPower 1            //1 = Yes, 0 = No,  Multiple sensors can be on one Power pin
#define DHT1Power 12          //boot fails if pulled high
#define DHT2Power 0           //Pin must be changed if used
#define DHT3Power 0           //Pin must be changed if used
#define DHT1Pin 26
#define DHT2Pin 0             //Pin must be changed if used
#define DHT3Pin 0             //Pin must be changed if used 


/* Pin assignments */
#define FrigControlPin 2
#define Blue0Pin 4
#define Blue1Pin 5
#define SCLpin 13
#define SDApin 14                        //outputs PWM signal at boot  
#define VSyncPin 15                      //outputs PWM signal at boot 
#define RXD2 16                          //UART 2 RX
#define TXD2 17                          //UART 2 TX
#define Green0Pin 18
#define Green1Pin 19
#define Red0Pin 21                        //Default SDA
#define Red1Pin 22                        //Default SCL
#define HSyncPin 23
#define ScaleClkPin 25              
#define ScaleOut4Pin 27
#define KeyboardDataPin 32
#define KeyboardClkPin 33
#define ScaleOut0Pin 34                   //Receive Only
#define ScaleOut1Pin 35                   //Receive Only
#define ScaleOut2Pin 36                   //Receive Only
#define ScaleOut3Pin 39                   //Receive Only

/* Common global defines */
#define MINUTES3 1000*60*3
#define MINUTES10 1000*60*10
#define MINUTES30 1000*60*30
#define DAYSECONDS 3600*24
#define DAYSECONDS 3600*24
#define TENSECONDMILS 1000*10
#define MINUTEMILS 1000*60
#define EpochYear 2023
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321
#define EpochUnix 1704096000  //Unix Time Jan 1, 2023 00:00
#define TimeZoneOffset -8  //Standard Time Zone Offset Hours  Los Angeles is 8 hours
#define EpochYear 2024
#define MaxDeltaTemp 3

/* Display Variables */
#define Command 1
#define Alarm 2
#define Scales 3
#define Normal 4
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

/* Key Definitions */
#define Esc 27
#define CarriageReturn 13
#define Backspace 8
#define Space 32

/* Color Definitions */
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
