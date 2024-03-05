/* Pin assignments */
#define FrigControlPin 2
#define Blue0GPIO 4
#define Blue1GPIO 5
#define DHTPIN2Power 12
#define DHTPIN1 13
#define DHTPIN1Power 14
#define VSyncGPIO 15
#define RXD2 16
#define TXD2 17
#define Green0GPIO 18
#define Green1GPIO 19
#define Red0GPIO 21
#define Red1GPIO 22
#define HSyncGPIO 23
#define ScaleClkPin 25
#define DHTPIN2 26
#define ScaleOut4Pin 27
#define KeyboardDataGPIO 32
#define KeyboardClkGPIO 33
#define ScaleOut0Pin 34
#define ScaleOut1Pin 35
#define ScaleOut2Pin 36
#define ScaleOut3Pin 39

/* Common global defines */
#define MINUTES3 1000*60*3
#define MINUTES10 1000*60*10
#define MINUTES30 1000*60*30
#define DAYSECONDS 3600*24
#define TENSECONDMILS 1000*10
#define MINUTEMILS 1000*60
#define EpochUnix 1704096000
#define TimeZoneOffset -8  //Standard Time Zone Offset Hours  Los Angeles is 8 hours
#define EpochYear 2024
#define MaxTempDelta 3

/* Display Variables */
#define Command 1
#define Alarm 2
#define Scales 3
#define Normal 4
int DisplayMode = Normal;

/* Alarm Flag bits */
#define Alarm0 0
#define Alarm1 1
#define Alarm2 2
#define Alarm3 3
#define Alarm4 4
#define Alarm5 5
#define Alarm6 6
#define Alarm7 7
#define Alarm8 8

/* Key Definitions */
#define Esc 27
#define CarriageReturn 13
#define Backspace 8
#define Space 32

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
