# **FREENOVE ESP32-WROOM Board**

For this project I selected the FREENOVE ESP32-WROOM developement board.  The main reason is the breakout board that is available for this board.  The breakout board is great.  I prefer to use a Arduino power supply instead of powering board from a type C USB connector.

![](ESP32_BreakOutBoard_Dist.jpg)

## ESP32 Breakout Board

Breakout Board Features:

- •Compatible models -> Freenove ESP32 / ESP32-S3 series. (Note: Boards from other brands are not compatible due to different pinout.)

- •GPIO status LED -> LED on if GPIO outputs/inputs high level, LED off if GPIO
  outputs/inputs low level.

- •Independent LED -> The status LED is driven by the chip instead of the GPIO so the GPIO will not be affected.

- •Power circuit -> Provide multiple 5V and 3.3V power outputs. (5V output up to 3A
  with external power supply.)

- •Terminal block and header -> Connect to all GPIO headers 

## ESP32-WROOM

The ESP Development board has:

- 32 bit processor that runs at 240 MHz

- 4 MB Flash and 520 KB of SRAM running at 80 MHz

- Wi-Fi and Bluetooth Interfaces

- Onboard RGB LED and Green LED

- Onboard Code Uploader

The ESP32 includes

- 8 Analog-to-Digital Converter (ADC) channels

- 3 SPI interfaces

- 3 UART interfaces

- 2 I2C interfaces

- 16 PWM output channels

- 2 Digital-to-Analog Converters (DAC)

- 2 I2S interfaces

- 10 Capacitive sensing GPIOs

The ADC (analog to digital converter) and DAC (digital to analog converter) features are assigned to specific static pins. However, you can decide which pins are UART, I2C, SPI, PWM, etc – you just need to assign them in the code. This is possible due to the ESP32 chip’s multiplexing feature.

## Power Distribution Board

I use these boards for my 3.3 Volt power and ground distribution.  There are not enough pins the ESP32 for all the power and ground signals.  These little boards make distributing the power and ground very easy.

## FREENOVE ESP32 Pin Out

![](FREENOVE_WROOM_Pinout.jpg)

## 
