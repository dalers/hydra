## **Hydra**

Hydra is a soil moisture data logger.

* [Source License (2-Clause BSD)](doc/LICENSE.md)
* [Contribution Guidelines](doc/CONTRIBUTING.md)
* [Code of Conduct](doc/CODE_OF_CONDUCT.md)

Hydra Data Logger Hardware Stack:

* [Arduino Uno R3](https://store.arduino.cc/products/arduino-uno-rev3)
* [Adafruit Data Logging Shield](https://www.adafruit.com/product/1141)
* [OSEPP 16x2 LCD Display and Keypad Shield](https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield)
* [Waveshare 5V Serial to RS-485 Adapter](https://www.waveshare.com/rs485-board-5v.htm)

Hydra RS485 Modbus Moisture Sensor:

* [Catnip Electronics Modbus RS485 Soil Moisture Sensor](https://www.tindie.com/products/miceuz/modbus-rs485-soil-moisture-sensor-2/). **Hydra is currently hard-coded for three moisture sensors, which must be pre-configured as Modbus ID 1, 2 and 3.**

## Support

Thank you for your interest in the Hydra project.

Please use [Discussions](https://github.com/dalers/hydra/discussions) to discuss or ask questions related to Hydra development and use.

If you find a software defect (bug), please enter it in [Issues](https://github.com/dalers/hydra/issues).

Pull Requests are welcome, but please present the need for your Pull Request first either in [Discussions](https://github.com/dalers/hydra/discussions) (for feature enhancements), or in [Issues](https://github.com/dalers/hydra/issues) (to correct software defects).

## Getting Started

* Assemble the Hydra data logger hardware stack following the [Hydra schematic](doc/Hydra-SCHEMATIC.pdf).
* Format a 2GB SD Card with FAT32 and insert it in the Arduino Data Logger Shield.
* Install the install the latest version of [Arduino IDE v2](https://wiki-content.arduino.cc/en/software).
* Install Arduino libraries (detailed instructions follow).
* Download Hydra source to your Arduino directory (either click the Code button to download a ZIP archive and extract to your Arduino directory, clone directly or fork).
* Connect the Hydra Data Logger Arduino USB port to your computer.
* Launch the Arduino IDE, open Open the [Hydra sketch](hydra.ino) and upload to the Arduino Uno R3.

## Arduino Libraries

Hydra uses the following third-party libraries:

* [RTClib](https://github.com/adafruit/RTClib) is used to access to the realtime clock (RTC) on the Arduino Data Logger Shield.
* [SdFat - Adafruit_Fork](https://www.arduino.cc/reference/en/libraries/sdfat) is used to access the SD Card on the Arduino Data Logger Shield.
* [Bounce2](https://www.arduino.cc/reference/en/libraries/bounce2) is used to debounce the pushbutton switches on the OSEPP 16x2 Text LCD & Keypad Shield.
* [LiquidCrystal](https://www.arduino.cc/reference/en/libraries/liquidcrystal) is the low-level interface to the LCD display on the OSEPP 16x2 Text LCD & Keypad Shield.
* [LCDKeypad](https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield) is an object-oriented interface to the pushbutton switches and LCD on the OSEPP 16x2 Text LCD & Keypad Shield.
* [ModbusMaster](https://www.arduino.cc/reference/en/libraries/modbusmaster) is the Modbus communication protocol stack for polling the Catnip RS485 Modbus Soil Moisture Sensors.

### RTClib

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "RTClib", and install the latest version of "RTClib by Adafruit" (e.g. v2.0.2). Also install Adafruit_BusIO when prompted (dependency of RTClib).

### SdFat

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "SdFat", and install the latest version of SdFat - Adafruit Fork (e.g. v2.1.2)

### Bounce2

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "Bounce2", and install the latest version of Bounce2 by Thomas O Fredericks (e.g. v2.71.2)

### LiquidCrystal

From the Arduino IDE, access menu Tools > Manage Libraries > Type: All, Topic: All, filter by "lcd", and install the latest version of LiquidCrystal by Arduino, Adafruit (e.g. v1.0.7).

### LCDKeypad

Download LCDKeypad.zip from <https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield>, and extract the LCDKeypad directory to \~/Arduino/libraries (in the "libraries" folder in your "Arduino Sketch Folder", where you will find the other libraries that have been installed).

### ModbusMaster

From the Arduino IDE, access menu: Tools > Manage Libraries > Type: All, Topic: All, filter by "ModbusMaster", and install the latest version of ModbusMaster by Doc Walker 4-20mA@wvfans.net (e.g. v2.0.1)


## Usage

* Connect sensors to the data logger.
* Apply power to the data logger. The LCD will display results of the startup self-test as performed: 1) Real-time clock (RTC), 2) SD Card, 3) Sensor 1, 4) Sensor 2, 5) Sensor 3. A startup error does not prevent logging (although, if the SD Card test fails there will be no log file).
* When the logging prompt appears on the LCD, press the **RIGHT** button on the LCD Display and Keypad Shield to start logging. The log filename will be automatially generated based on the current date and time (ddmmhhmmss.csv). A star will blink at the top left of the LCD when logging.
* When logging, press the **RIGHT** again to stop logging.
* Disconnect power, remove the SD Card and access the log file on a computer using an SD Card reader.
