# MCP9808 Precision Temperature Sensor

This MCP9808 digital temperature sensor is low cost temperature sensor with a typical accuracy of ±0.25°C over the sensor's -40°C to +125°C range and precision of +0.0625°C.   It has a wide voltage range of 2.7  to 5.5 volts.  You can typically buy them for about 6 dollars.   The following is a picture of the typical device.  The device works on the I2C bus that is explained in the next section.

![](Picture.jpg)

## I2C Bus

The I2C bus is a standard **bidirectional interface** that uses a controller, known as the master, to communicate with slave devices. A slave may not transmit data unless it has been addressed by the master. Each device on the I2C bus has a specific device address to differentiate between other devices that are on the same I2C bus.  The I2C bus can support 255 different addresses with a two wire interface.

All Arduino compaitible boards support the I2C bus.  The MCP9808 has 3 address pins so you can connect up to 8 on the I2C bus.  The base address 0x18.

The I2C bus is supported by the Arduino Wire library.

## MCP9808 vs DHT Sensors

I decided to add the capability to use either Digital Humidity and Temperature (DHT) or  MCP9808 sensors.  I found that the DHT22 sensors had temperature biases upto 2 degrees and they seem to drift.    The following figure compares the MCP to the DHT22.

![](Comparison.jpg)

## Implementation

I ran five wires to each sensor (+3.3V, Gnd, SDA, SCL and A0).  On one sensor A0 was connected to 3.3V and the other sensor is connected to GND.  The addresses of the two sensors are hard wired to 0x18 and 0x19.  The following is a picture of the wired sensors.  I found that the ground wire had to be run to 0x18 sensor (sensor #1) to get reliable readings.  In the picture these sensors are generic sensors.  On Adafruit sensors the Alert pin is where the A0 pin is.  For the Adafruit sensors I used a 4 pin connector and ran a single wire to the A0 address line.

![](SensorWiring.jpg)

Because the IC2 bus runs at a high rate (100 kHz)  the SDA and SCL wires should be kept to about a foot or less.  The other wires can be longer.

## Software

You can use upto 3 sensors (any mix of MCP9808 and DHT sensors). The "constants.h" file is edited to select the number of sensors and the type of sensors used.

The SDA and SCL pins are also defined in the "constants.h" file.  The SCL pin is 13 and the SDA pin is 14, but these can be assigned to almost any GPIO pin.

The MCP9808 sensors are assigned sensor numbers first.  For example if you have 2 MCP9808 sensors and 1 DHT sensor, Sensor #1 and Sensor #2 are the MCP9808 and Sensor #3 is the DHT sensor.
