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

   2022-08-15 installed v2.1.1 on T500, also installed Adafruit BusIO as dependency 2022-08-16 updated T420 to v2.1.1 (also updated BusIO)

### Bounce2

Bounce2 is used to debounce the pushbutton switches on the OSEPP 16x2 Text LCD & Keypad Shield.

1. Install "Bounce2" (<https://www.arduino.cc/reference/en/libraries/bounce2/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "Bounce2"

   > Install Bounce2 by Thomas O Fredericks v2.71.2

   2022-08-15 installed v2.71.0 on T500 (highest ver available) 2022-08-16 installed v2.71.0 on T420 (highest ver available)

### ModbusMaster

ModbusMaster is used to access the Catnip RS485 (Modbus) moisture sensors.

1. Install "ModbusMaster" (<https://www.arduino.cc/reference/en/libraries/modbusmaster/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "ModbusMaster"

   > Install ModbusMaster by Doc Walker 4-20mA@wvfans.net v2.0.1

   2022-08-15 installed v2.0.1 on T500 2022-08-16 installed v2.0.1 on T420

### SdFat

SdFat is used to access the SD Card on the Arduino Data Logger Shield.

1. Install "SdFat - Adafruit Fork" (<https://www.arduino.cc/reference/en/libraries/sdfat/>)

   IDE > Tools > Manage Libraries > Type: All, Topic: All, filter by "SdFat" (by Adafruit)

   > Install SdFat by Adafruit v2.1.2

   2022-??-?? "SdFat" (by Bill Greiman) v2.1.2 installed on HP Elitebook and/or T420 ?? 2022-08-15 installed v1.5.1 on T500 2022-08-16 installed v1.5.1 on T420

### LCDKeypad Lib

LCDKeypad is used to access the pushbutton switches and LCD on the OSEPP 16x2 Text LCD & Keypad Shield (it encapsulates the standard LCD library for LCD access).

1. Download LCDKeypad.zip from <https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield>
2. Extract LCDKeypad\\ from LCDKeypad.zip to \~/Arduino/libraries

   2022-08-15 installed on T500 2022-08-16 already installed on T420