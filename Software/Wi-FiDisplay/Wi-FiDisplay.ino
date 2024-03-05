/* ESP32 board select the Wemos D1 R32 board Minimal SPIFFS for this code */
#include <Arduino.h>
#include <TimeLib.h>
#include <WiFi.h>                 //Wi-Fi Library
#include <ESPmDNS.h>              //Dynamic Name Service Library
#include <ESP_Mail_Client.h>      //Email and Text Message Libray
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <WebSerialLite.h>
#include "KegMonitor.h"
#include "Passwords.h"

/* Debug Variables */
boolean printSerial = true;                             /* Display Serial 2 data (ESP25) */
const char* newhostname = "KegMonitor";

#define numscales 5                            /* Number of Scales currently connected */
#define MaxBufferSize 9000                     /* Maximum HTML page size */
#define MINUTES1 60000                         /* Number of milliseconds in 1 Minute */
#define MINUTES4 MINUTES1*4                    /* Number of milliseconds in 4 Minute */
#define MINUTES5 MINUTES1*5                    /* Number of milliseconds in 5 Minutes */
#define HOURMILLS MINUTES1*60                  /* Number of milliseconds in 1 hour */   
#define DAYMILLS HOURMILLS*24                  /* Number of milliseconds in a day */ 

/* Hardware Pin assignments */
#define RXD2 16
#define TXD2 17

const String imageString = "data:image/bmp;base64,Qk0uCAAAAAAAAHYAAAAoAAAAZwAAACYAAAABAAQAAAAAALgHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAgAAAAICAAIAAAACAAIAAgIAAAICAgADAwMAAAAD/AAD/AAAA//8A/wAAAP8A/wD//wAA////APhmj/////////////////////////////////////////////////////////////////CGd2/////////////////////////////////////////////////////////////////wZ3dn////////////////////////////////////////////////////////////////8Gd3Z/////////////////////////////////////////////////////////////////Bnd2eIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiI/wZ3dmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZm8Gd3Znd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dnBnd2Z3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dgZ3dmd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3YGd3Znd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d2Bnd2Z3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dgZ3dmd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3iHd3eId3d3d3d3d3d3d3YGd3Znd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d4h3d3iHd3d3d3d3d3d3d2Bnd2Z3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3eId3d4h3d3d3d3d3d3d3dgZ3dmd3d3d3d3d/h3d373d393d3/393f/+HeId493d/h3d373iP+HeI/4d3d3d3d3d3d3YGd3Znd3d3d3d3ePd3d4h3d/d3ePd/d/h3+HiH73d3ePd3d4h493+Hj3f4d3d3d3d3d3d2Bnd2Z3d3d3d3d3f3d3f4d3f3d++Hf373d3d4h/h3d3f3d3f4eId494h3j3d3d3d3d3d3dgZ3dmd3d3d3d3d3j///93d393f4d39493d3ePj3d3d3j///93iHfveId+93d3d3d3d3d3YGd3Znd3d3d3d3d+93f4d3d///+Hd/fvd3d3iPh3d3d+93f4d4h3j3iHePd3d3d3d3d3d2Bnd2Z3d3d3d3d3d/h+93d3f3d3j3f3f4d/d4jvh3d3d/h+93ePd/h493+Hd3d3d3d3d3dgZ3dmd3d3d3d3d3ePeId3d393d39393f/+HeIfvh3d3ePeId3iP/3eI//d3d3d3d3d3d3YGd3Znd3d3d3d3d3f493d3d/d3d/d3d3d3d3iHd3d3d3f493d3d3d3d3d3d3d3d3d3d3d2Bnd2Z3d3d3d3d3d3iPd3d3f3d3j3d3d3d3d4h3d3d3d3iPd3d3d3d3d3d3d3d3d3d3d3dgZ3dmd3d3d3d3d3d++Hd3d3////d393d3d3eId3d3d3d++Hd3d3d3d3d3d3d3d3d3d3d3YGd3Znd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d2Bnd2Z3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dgZ3dmd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3YGdnZnd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d2BmZmZ3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dgZmZmd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3YGdmZnd3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d2B2dmZ3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3dg92ZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmYP/4iIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIdmZmD////////////////////////////////////////////////////////////////3ZmZg////////////////////////////////////////////////////////////////+GZmYP////////////////////////////////////////////////////////////////92ZoD/////////////////////////////////////////////////////////////////93jw";

/* HTML Variables */
char HTMLpage[MaxBufferSize];                           /* Page to send to browser */
int indx[30];                                           /* Ptrs into buffer to insert variables */
int len = 0;                                            /* size of index_html */
String blank = "                       ";               /* Blank string */
int AlarmCnt = 0;
String AlarmStr = "";
char tmpbuffer[20];        

/* Loop time Variables */
unsigned long curtime;                                  /* Current Loop Time */
unsigned long bTime = 0;                                /* Time to blink LED */
unsigned long wTime = MINUTES4;                         /* Time to check WiFi Connection Status */
unsigned long tTime = 0;                                /* Time to Send Time */  
unsigned long nTime = MINUTES1;                         /* Time to check Time */
float temperature[] = {35, 34, 34};                     /* Average, #1, #2 */
boolean ValidTime = false;

/* Temperature Variables */
boolean AlarmFlag = true;                              /* Set True so no alarms until data received */
int freezerOn = 0;                                      /* freezer On flag, 0=Off, 1=On */
const int Mintemp = 30;                                 /* Minimum Freezer Temperature */
const int Maxtemp = 40;                                 /* Maximum Freezer Temperature */

/* WiFi Variables */
String IPaddr = "";                                     /* IP Address */ 
float RSSIlvl = 31.0;                                   /* Current RSSI level in dBm */
boolean WiFiflag = false;                               /* Flag if WiFi is connected */

/* Display Variables */
float Alcohol[] = {0.0, 0.0, 0.0, 0.0, 0.0};                                   /* Percent Alcohol */
String BeerNames[] = {"Unknown", "Unknown", "Unknown", "Unknown", "Unknown"};  /* Titles for each Tap */ 
int DaysKegged[] = {0, 0, 0, 0, 0};                                           /* Number of days the beer has been in the keg */
int GlassesLeft[] = {25, 9, 25, 35, 8};                                       /* Array of glasses left in each keg */
unsigned long DataTime = MINUTES1;
boolean ConnectedFlag = false;                                                    

/* Define Structures and Objects */
AsyncWebServer server(80);

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;
/* Declare the message class */
SMTP_Message message;
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);
// This will be displayed to identify the firmware
const char myVer[] = __DATE__ " @ " __TIME__;

/* -------------------------------------------------------------------------------------------------------------- */
void setup() {
// Initialize serial and wait for port to open:
  Serial.begin(115200);                                                          /* Start Serial 0 Port */  
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);                                   /* Start Serial 2 Port */
  Serial.println("Serial Ports Ready");                                          /* Set Normal Status */

  Serial.println();
  Serial.println("ESP32 Setup");
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));

/* Setup Pin Modes */
   pinMode (LED_BUILTIN, OUTPUT);                                                /* On Board LED */
   
/* Copy HTML data to an array */  
   ReadHTMLPage();                                                            /* Copy HTML data from KegMonitor.h */
   Serial.print ("Software Build Date "); Serial.println(myVer);
   InitWiFi();                                                                   /* Intialize WiFi */
/* Get and Set RTC to Local Time */
   if (WiFiflag) {
     Serialprintln("Getting Current Time");
     if (!SetRTCTime()) {
       Serialprintln("Failed to obtain Time");
     }
     else {
       ValidTime = true;
       Serialprintln("Current Time is " + DateTimeStr(1));
     }
     AsyncElegantOTA.begin(&server);
     Serial.println("Elegant OTA started");
     WebSerial.begin(&server);
     Serial.println("WebSerial started");
     WebSerial.onMessage(recvMsg);    /* Attach Message Callback */
    
    /* Start HTML Server Functions */
     InitServerFunctions();
     Serialprintln( "HTTP Async server started" );
    
     MDNS.addService("http", "tcp", 80);
   }  
/* Stagger Event Times by 100 millis */   
   curtime = millis();
   bTime = curtime + 100;                               /* Time to blink LED */
   wTime = curtime + 200 + MINUTES5;                    /* Time to check WiFi Connection Status */
   tTime = curtime + 300 + MINUTES1;                    /* Time to Send Time */  
   nTime = curtime + 400;                               /* Time to check Time */
   Serialprintln("ESP32 Setup Complete");
}
/* ------------------------------------------------------------------------------------------------------------- */
void loop() {
  curtime = millis();
//  server.handleClient();
  
/* Check if information received on Port 2 */
  if (Serial2.available() >> 0) {                    /* Check if anything is in the buffer */
    String s = Serial2.readString();                 /* Read data in serial port */
    if (printSerial) Serialprintln("Rcvd: " + s);       /* Print that something was Received */
    int x = s.length();                              /* Get the length of the string */
    int start; int last;                             /* Declare Variables */
    for (int i = 0; i < x; i++){                     /* Search until a command or the end of the string */
      if (s.charAt(i) == '&') {                      /* Start of cmd string */
        start = i;                                   /* save start */
      }
      if (s.charAt(i) == ',') {                      /* End of command */
        last = i;                                    /* Save end of cmd ptr */
        ProcessCmd(s.substring(start, last));        /* Process the command */
      }
    }    
  }  
  
/* Check System Time */
  if (curtime > (nTime)) {                                               /* If it is time to update time */
    nTime = curtime + DAYMILLS;
    if (WiFiflag) {                                                     /* Is WiFi connected? */
      if (timeStatus() == timeSet) {
        Serialprintln("Time is Synced");
      }
      else {
        Serialprintln("Time needs to be Resynced");
        if (SetRTCTime()) {
          Serialprintln("Time Resync Successful");
        }
        else {
          Serialprintln("Time Resync Failed");
        }
      }
    }
  } 
/* Send WiFi and Time status */
  if (curtime > (tTime)) {                                              /* Is it time to check time */
    tTime = tTime + MINUTES1;             
    ConnectedFlag = (curtime > (DataTime + MINUTES1)) ? false : true;
    if (ConnectedFlag == false) processAlarms();
    SendWiFi();
    if (ValidTime) {                                                     /* Is time of day valid? */
      SendTime();                                                        
    }
  }
/* Check WiFi connection */  
  if (curtime > wTime) {                                              /* if it is time to check the Wifi */
    wTime = wTime + MINUTES5;                                          /* Check Wifi again in 5 minutes */
    if (WiFi.status() == WL_CONNECTED) {                               /* is WiFi Connected */  
      RSSIlvl = WiFi.RSSI();                                           /* Get current signal level */
      IPaddr = WiFi.localIP().toString();                              /* Get the IP Address */
      WiFiflag = true;                                                 /* Set Wifi is Connected */
      SendWiFi();                                                      /* Send WiFi Status */
    }  
    else {                                                             /* WiFi is not connected */
      WiFi.disconnect();                                               /* Disconnect Wifi */
      RSSIlvl = 31.0;
      SendWiFi();
      WiFi.begin(ssid,password);                                       /* Try to reconnect */
      long timeout = millis();                                         /* Set Timeout to current time */
      while (WiFi.status() != WL_CONNECTED) {                          /* Loop until connected */
        delay(200);                                                    /* wait */
        Serial.print(".");                                             /* Show connecting dot */
        if (millis() - timeout > 300000) {                             /* Did not connect */  
          WiFiflag = false;                                            /* Set Wifi is not Connected */
          wTime = wTime + HOURMILLS;                                   /* Check Wifi again in 1 hour */
          break;                                                       /* Exit loop */
        }
      }
    }
  }
/* blink1 LED */
  if (curtime > bTime) {
    bTime = bTime + 1000;
    if (WiFiflag) {                                                            /* if connected blink LED */
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }  
    else {                                                                     /* WiFi not connected */  
      digitalWrite(LED_BUILTIN, HIGH);                                         /* Set LED On */                           /* Set LED to Red */
    }  
  }
}   
