# **Hydra**

Hydra is an datalogger for Modbus moisture sensor data.

The datalogger hardware consists of:

* Arduino Uno
* Adafruit Data Logging Shield (includes RTC with battery-backup)
* OSEPP 16x2 LCD Display and Keypad Shield
* Waveshare 5V Serial to RS-485 Adapter

# **Getting Started**

## Install Libraries

### RTClib

RTClib is required to access the realtime clock chip on the Arduino Data Logger Shield.

1. Install "RTClib" (<https://github.com/adafruit/RTClib>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "RTClib"

   > Install RTClib by Adafruit v2.0.2

### Bounce2

Bounce2 is used to debounce the pushbutton switches on the OSEPP 16x2 Text LCD & Keypad Shield.

1. Install "Bounce2" (<https://www.arduino.cc/reference/en/libraries/bounce2/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "Bounce2"

   > Install Bounce2 by Thomas O Fredericks v2.71.2

### ModbusMaster

ModbusMaster is used to access the Catnip RS485 (Modbus) moisture sensors.

1. Install "ModbusMaster" (<https://www.arduino.cc/reference/en/libraries/modbusmaster/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "ModbusMaster"

   > Install ModbusMaster by Doc Walker 4-20mA@wvfans.net v2.0.1

### SdFat

SdFat is used to access the SD Card on the Arduino Data Logger Shield.

1. Install "SdFat - Adafruit Fork" (<https://www.arduino.cc/reference/en/libraries/sdfat/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "SdFat"

   > Install SdFat - Adafruit Fork v2.1.2

### LCDKeypad Lib

LCDKeypad is used to access the pushbutton switches and LCD on the OSEPP 16x2 Text LCD & Keypad Shield (it encapsulates the standard LCD library for LCD access).

1. Download LCDKeypad.zip from <https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield>
2. Extract LCDKeypad\ from LCDKeypad.zip to \~/Arduino/libraries

