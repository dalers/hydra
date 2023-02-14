## **Hydra**

Hydra is a soil moisture monitor and data logger.

* [Source License (2-Clause BSD)](doc/LICENSE.md)
* [Contribution Guidelines](doc/CONTRIBUTING.md)
* [Code of Conduct](doc/CODE_OF_CONDUCT.md)

Hydra Hardware Stack:

  * [Arduino Uno R3](https://store.arduino.cc/products/arduino-uno-rev3)
  * [Adafruit Data Logging Shield](https://www.adafruit.com/product/1141)
  * [OSEPP 16x2 LCD Display and Keypad Shield](https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield)
  * [Waveshare 5V Serial to RS-485 Adapter](https://www.waveshare.com/rs485-board-5v.htm)
  * [Catnip Electronics Modbus RS485 Soil Moisture Sensor](https://www.tindie.com/products/miceuz/modbus-rs485-soil-moisture-sensor-2/)

Refer to the [Hydra schematic](doc/Hydra-SCHEMATIC.pdf) for interconnection.


## Getting Started

### Install Arduino IDE 2

Hydra is developed using the Arduino IDE (Arduino IDE v2 is recommended).

### Install Libraries

The following libraries must be installed to compile Hydra:

* RTClib (and dependency Adafruit_BusIO)
* SdFat_-_Adafruit_Fork
* Bounce2
* LiquidCrystal
* LCDKeypad
* ModbusMaster

#### RTClib

RTClib (<https://github.com/adafruit/RTClib>) provides access to the realtime clock chip on the Arduino Data Logger Shield.

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "RTClib", and install the latest version of "RTClib by Adafruit" (e.g. v2.0.2)

#### SdFat

SdFat (<https://www.arduino.cc/reference/en/libraries/sdfat/>) provides access to the SD Card on the Arduino Data Logger Shield.

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "SdFat", and install the latest version of SdFat - Adafruit Fork (e.g. v2.1.2)

#### Bounce2

Bounce2 (<https://www.arduino.cc/reference/en/libraries/bounce2/>) provides software debounce of the pushbutton switches on the OSEPP 16x2 Text LCD & Keypad Shield.

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "Bounce2", and install the latest version of Bounce2 by Thomas O Fredericks (e.g. v2.71.2)

#### LiquidCrystal

LiquidCrystal (<https://www.arduino.cc/reference/en/libraries/liquidcrystal/>) provides low-level access to the LCD display on the OSEPP 16x2 Text LCD & Keypad Shield.

From the Arduino IDE, access menu Tools > Manage Libraries > Type: All, Topic: All, filter by "lcd", and install the latest version of LiquidCrystal by Arduino, Adafruit (e.g. v1.0.7).

#### LCDKeypad

LCDKeypad provides an object oriented library for interaction with the pushbutton switches and LCD on the OSEPP 16x2 Text LCD & Keypad Shield.

Download LCDKeypad.zip from <https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield>, and extract the LCDKeypad directory to \~/Arduino/libraries

#### ModbusMaster

ModbusMaster (<https://www.arduino.cc/reference/en/libraries/modbusmaster/>) is the communication protocol stack to access to the Catnip RS485 Modbus Soil Moisture Sensors.

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "ModbusMaster", and install the latest version of ModbusMaster by Doc Walker 4-20mA@wvfans.net (e.g. v2.0.1)


### Usage

* Assemble the Hydra hardware stack following the [Hydra schematic](doc/Hydra-SCHEMATIC.pdf).
* Format a 2GB SD Card with FAT32 and insert it in the Arduino Data Logger Shield.
* Verify and upload the hydra sketch to the Arduino
* Use the Serial Monitor in the IDE to verify correct startup
* Press the **RIGHT** button on the LCD Display and Keypad Shield to start logging. Press **RIGHT** again to stop logging.
* Disconnect power to the Arduino and move the SD Card to a PC to access the log file.

