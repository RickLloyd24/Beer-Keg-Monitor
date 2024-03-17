# DHT22/AM2302

The Digital Humidity and Temperature (DHT)  Sensor Module is compact and easy to use module.  

## Features:

- Main Chip: AOSONG AM2302 High Sensitive Temperature Humidity Sensor
- Single-bus digital signal output, bidirectional serial data
- Temperature range: -40 to 80 degree celsius, 
- Temperature measurement accuracy: +/- 0.5℃ degree celsius
- Humidity measuring range: 0~100%RH, 
- Humidity measurement accuracy: ±2%RH

## Specifications

- Power supply 3.3-6V DC

- Sensing element Polymer capacitor

- Operating range humidity 0-100%RH; temperature -40~80 Celsius

- Accuracy humidity +-2%RH(Max +-5%RH); temperature <+-0.5 Celsius

- Resolution or sensitivity humidity 0.1%RH; temperature 0.1 Celsius

- Repeatability humidity +-1%RH; temperature +-0.2Celsius

- Humidity hysteresis +-0.3%RH

- Long-term Stability +-0.5%RH/year

- Sensing period Average: 2 seconds

## Software

The software will work with any DHT sensors (DHT11, DHT21, DHT22, AM2301, or AM2302). You can use upto 3 sensors (any mix of MCP9808 and DHT sensors). The "constants.h" file is edited to select the number of sensors and the type of sensors used.

I tried multiple DHT sensor libraries.  They either were unreliable (sensor temperature got stuck) or they were blocking which caused the display to blink when the sensor was read.  To solve this problem I wrote my own library <DHTSimple.h>.  This library is avalible on GitHub.

The DHT sensors can be connected to available GPIO pin.  The pin numbers are defined in the "constants.h" file.  If MCP9808 sensors are installed, the sensor numbers will be assigned to these devices first.
