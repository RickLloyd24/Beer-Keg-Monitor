# Beer Keg Monitoring System

Build your own beer keg monitoring system that keeps track of how many glasses are
left in each keg.  The entire system costs
less than $100 for 5 beer taps.

## Features:

- ·         Tracks level of beer left.

- ·         Measurers temperature.

- ·         Controls Beer freezer temperature

- ·         Provides email and text message notifications.

- ·         Provides local and remote displays.

- ·         Monitors up to 8 beer taps.

## Introduction

Currently there are two types of system to measure how much beer you have left

1)      Flow measuring.

2)      Weight of Keg.

Flow measuring systems are cheaper (6 taps about $500) but they have limited utility and you have to clean them every time you clean your beer lines (every 6 weeks).

Weight of keg systems are none intrusive, but very expensive ($1000 for 5 taps).  They are not integrated with your entire system.

This system is a low cost weight measuring system.

## System Overview

The following figure is a high level over of the whole project:

![](C:\Users\rickl\OneDrive\Documents\GitHub\Beer-Keg-Monitor\Keg_Monitor_System.jpg)

A few notes on the system design.  I keep my beer kegs in a chest freezer, so I have to control the power so the beer does not freeze.  That is why the relay is included in the design.  

The beer freezer is in the pool shack which does not have WiFi.  To get WiFi, I installed an old ESP32 board under the eave and ran a cable between the two processors.  The talk to each other on a serial interface.  If you have WiFi where your beer is stored you can get rid of the ESP32 board and the serial interface.  You can install all the software on the main WROOM board.

When I first installed the system I had trouble with the DHT sensor.  It would hang up on one temperature and not change.  One time I had frozen beer and the other time I had warm beer, both are bad.  To fixe this problem I used two sensors and power them from a GPIO pin.  This way I can reset the sensors.  Since I made this change I have not had any problems.  I keep my beer freezer at 33 degrees +/- 0.6 degrees.  If you use a refrigerator to store your beer you don't need the relay or the second DHT sensor.

## Bill of Materials

The following is the bill of Materials for the entire system:

<style>
</style>

| **Item** | **Qty** | **Description**               | **Price**     | **Total**     |
| -------- | ------- | ----------------------------- | ------------- | ------------- |
| 1        | 5       | Scale Plate                   | $       2.50  | $    12.50    |
| 2        | 5       | HX711 Load Cell<br> Amplifier | $       2.00  | $    10.00    |
| 3        | 20      | Load Cell                     | $       0.80  | $    16.00    |
| 4        | 1       | PS2 Keyboard                  | $       5.00  | $       5.00  |
| 5        | 1       | VGA Monitor                   | $    10.00    | $    10.00    |
| 6        | 2       | DHT 22/AM2302 Sensor          | $       3.33  | $       6.67  |
| 7        | 1       | ESP32 Breakout Board          | $    13.00    | $    13.00    |
| 8        | 1       | ESP32 Development Board       | $       8.50  | $       8.50  |
| 9        | 1       | VGA Breakout Board            | $       7.50  | $       7.50  |
| 10       | 1       | PS2 Breakout Board            | $       4.00  | $       4.00  |
| 11       | 1       | Relay                         | $        2.00 | $        2.00 |
|          |         | **Total**                     |               | $    95.17    |

# Theory of Operation

The theory of operation is quite simple.  The data you need is:

1)      Scale reading with nothing on the scale (Empty Scale Quanta)

2)      Scale reading with a full keg of beer on the scale (Full Scale Quanta)

3)      Scale reading of current keg (Current Scale Quanta)

4)      Weight of a full keg (Full Keg Weight)

5)      Weight of an empty keg (Empty Keg Weight)

The system automatically measures the first three items, all you need to do is measure the full key weight and the empty keg weight.  I just use a bathroom scale for this.

Reading Per Ounce = (Full Scale Quanta – Empty Scale Quanta) / (Full Keg Weight * 16)

Current Ounces = (Current Scale Quanta – Empty Scale Quanta) / Reading Per Ounce

Glasses Left = (Current Ounces – Empty Keg Weight) / 12 Ounces per glass

# Software

The following are the Arduino libraries I used in this project.  With the information provided in this article an experienced Arduino programmer should be able to develop the software.  I hope to be able to publish another article or post the software on GitHub.

#include "fabgl.h"                //VGA Monitor and Keyboardlibrary

#include <dhtESP32-rmt.h>         //DHT Sensor Library

#include "FS.h"                   //File System Library (required for LittleFS)

#include "LittleFS.h"             //Little File System Library 

#include <WiFi.h>                 //Wi-Fi Library

#include <ESPmDNS.h>              //Dynamic Name Service Library

#include <ESP_Mail_Client.h>      //Email and Text Message Library

#include "NTP.h"                  //Network Time Protocol library

#include "WiFiUdp.h"              //Universal Data Package library (required for NTP)

#include <ArduinoOTA.h>           //Over-The-Air (OTA) Updates

#include <WiFiClient.h>           //Wi-Fi Client Library

#include <WebServer.h>            //Wi-Fi Server Library

#include <ElegantOTA.h>           //Easy to Use OTA updater

You notice that there is not a HX711 library.  I felt this library was overly complicated and I did not have enough pins to run both data and clock to all scales.  So I wrote my own code with a single clock to drive all the scales. The library is HX711_Single_Clk and it is available on GitHub.

## Lessons Learned

1)    Buy solid wire instead of stranded.  Much easier to work with.

2)    Use an Ohm meter to check everything out.  It can save a lot of troubleshooting
time. 

3)    Buy the ESP32 Development Boards, instead of the UNO form factor boards (i.e. ESP32 D1 R32 or ESP8266 D1 R2).  Much easier to work with, better WiFi, and faster.

4)    If you need a lot of memory and pins go with the ESP32-S3 development boards.

5)    Use ESP32 instead of 8266.  About the same price but ESP32 has two cores
and runs faster and is more popular. 

6)    Use breakout boards the screw terminals, much easier and more reliable than the solder boards.

## Hardware

Check out the hardware section for all the intimate hardware details
