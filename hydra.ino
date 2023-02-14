/*
  Hydra Soil Moisture Sensor Data Logger
  (C) 2023 Dale Scott
  License 2-Clause BSD

*/

// ***********************************
// LIBRARIES
// ***********************************

// serial library (built-in)  // https://www.arduino.cc/reference/en/language/functions/communication/serial/

#include <Bounce2.h>          // https://www.arduino.cc/reference/en/libraries/bounce2/
#include <ModbusMaster.h>     // https://www.arduino.cc/reference/en/libraries/modbusmaster/
#include <SoftwareSerial.h>   // https://docs.arduino.cc/learn/built-in-libraries/software-serial
#include <SdFat.h>            // https://www.arduino.cc/reference/en/libraries/sdfat/
#include <LiquidCrystal.h>    // https://www.arduino.cc/reference/en/libraries/liquidcrystal/
#include <LCDKeypad.h>        // https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield
#include <RTClib.h>           // https://www.arduino.cc/reference/en/libraries/rtclib/

// ***********************************
// GLOBALS
// ***********************************

#define DRIVER_ENABLE A1            // RS-485 transceiver direction

bool logging = false;               // logging state - true if logging
uint16_t samples = 1;               // sample counter, uint16_t = 45d @ 1min sample period
uint8_t r;	                        // Modbus read status

SoftwareSerial softSerial(2, 3);    // Pin 2 = RX, Pin 3 = TX
Bounce button = Bounce();
ModbusMaster sensor;
SdFat sd;
LCDKeypad lcd;
RTC_PCF8523 rtc;

// SD Card
const int chipSelect = A2;          // TODO change to #define (same as DRIVER_ENABLE)

// ModbusMaster callback functions
// enable RS-485 driver before sending
void preTransmission() {
  digitalWrite(DRIVER_ENABLE, HIGH);
}

// disable RS-485 driver after sending
void postTransmission() {
  digitalWrite(DRIVER_ENABLE, LOW);
}

// log sample period
long logMillis = 60000;           // log period in ms (1min=60000, 5min=300000, 10min=600000, 1hr=3600000)
long prevLogMillis = 0;           // millis of last log sample

// ***********************************
// SETUP
// ***********************************

void setup() {
  Serial.begin(9600);               // Console
  Serial.println(F("#Starting"));   // output startup notice to serial port

  softSerial.begin(19200);          // RS-485 software UART
  pinMode(DRIVER_ENABLE, OUTPUT);   // RS-485 driver control

  pinMode(A0, INPUT_PULLUP);        // LCD keypad "RIGHT" button TODO why enable internal pullup? (will be in parallel with 3K on PCB)
  button.attach(A0);                // attach bounce object to A0 (RIGHT)
  button.interval(5);               // debounce time 5ms

  // show starting up on LCD
  lcd.begin(16, 2);
  lcd.clear();                      // sets cursor to (col 0, row 0)
  lcd.print(F("Hydra Controller"));
  lcd.setCursor(0,1);
  lcd.print(F("FW 0.0.4"));
  delay(2000);                      // display fw version for 2+ seconds
  
  // Probe RTC
  if (rtc.begin()) {
    Serial.println(F("#RTC found"));
    
    if (! rtc.initialized() || rtc.lostPower()) {
      Serial.println(F("#RTC NOT initialized"));
      // TODO manage RTC not initialized
    } else {
      DateTime now = rtc.now();
      Serial.print(F("#RTC time "));
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      Serial.print(F(" "));
      Serial.print(now.hour(), DEC);
      Serial.print(F(":"));
      Serial.print(now.minute(), DEC);
      Serial.print(F(":"));
      Serial.print(now.second(), DEC);
      Serial.println();
    }
  } else {
    Serial.println(F("#RTC NOT found"));
    // TODO set flag if RTC not found
  }

  // Probe SD Card
  if(sd.begin (chipSelect, SPI_HALF_SPEED)) {
    Serial.println(F("#SD Card found"));
  } else {
    Serial.println(F("#SD Card NOT found"));
    // TODO set flag if SD Card not found
  }

  // Probe Moisture Sensor 1
  sensor.begin(1, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO is this needed?)
  sensor.postTransmission(postTransmission);

  r = sensor.readInputRegisters(0, 3);
  if (0 == r) {
    Serial.print(F("#Sensor 1 found. Moisture: "));
    Serial.print(sensor.getResponseBuffer(0));
    Serial.print(F(", Temp: "));
    Serial.println(sensor.getResponseBuffer(1));
  } else {
    Serial.println(F("#Sensor 1 NOT found"));
    // TODO set flag if sensor 1 not found
  }
  delay(200);                                 // sensors need delay before next read
    
  // Probe Moisture Sensor 2
  sensor.begin(2, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO is this needed?)
  sensor.postTransmission(postTransmission);

  r = sensor.readInputRegisters(0, 3);
  if (0 == r) {
    Serial.print(F("#Sensor 2 found. Moisture: "));
    Serial.print(sensor.getResponseBuffer(0));
    Serial.print(F(", Temp: "));
    Serial.println(sensor.getResponseBuffer(1));
  } else {
    Serial.println(F("#Sensor 2 NOT found"));
    // TODO set flag if sensor 2 not found
  }
  delay(200);                                 // sensors need delay before next read
	  
  // Probe Moisture Sensor 3
  sensor.begin(3, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO is this needed?)
  sensor.postTransmission(postTransmission);

  r = sensor.readInputRegisters(0, 3);
  if (0 == r) {
    Serial.print(F("#Sensor 3 found. Moisture: "));
    Serial.print(sensor.getResponseBuffer(0));
    Serial.print(F(", Temp: "));
    Serial.println(sensor.getResponseBuffer(1));
  } else {
    Serial.println(F("#Sensor 3 NOT found"));
    // TODO set flag if sensor 3 not found
  }
  delay(200);                                 // sensors need delay before next read (probably not needed here)
	  
  // startup complete, show logging prompt
  lcd.setCursor(0,1);
  lcd.print(F("Start Logging >R"));

  Serial.println(F("#Startup Complete"));
}

// ***********************************
// MAIN LOOP
// ***********************************

void loop() {

  // TODO handle probe failure of RTC, SD Card, or Moisture Sensors
  
  button.update();
  if(button.fell()) {             // if RIGHT was pressed and released
    Serial.println(F("#RIGHT pressed"));
  
    logging = !(logging);
    Serial.print(F("#Logging state: ")); Serial.println(logging);

    if (logging == true) {
      lcd.setCursor(0,1);
      lcd.print(F("Stop Logging  >R"));
      // write header to logfile
      // TODO generate log filename using current date/time e.g. "ddhhmmss.csv"
      File logfile = sd.open("logfile.csv", FILE_WRITE);
      logfile.println(F("DateTime,ID,S1-M,S1-T,S2-M,S2-T,S3-M,S3-T"));
      logfile.close();

    } else {
      lcd.setCursor(0,1);
      lcd.print(F("Start Logging >R"));
    }
  }
    
  uint16_t moisture = 0;                   // sensor moisture value
  int16_t  temperature = 0;                // sensor temperature value
  uint8_t  r = 0;                          // sensor return value
  unsigned long currentMillis = millis();  // current sample time

  if (logging && (currentMillis - prevLogMillis > logMillis)) {

    prevLogMillis = currentMillis;

    DateTime now = rtc.now();   // get current date/time TODO copying from RTC to register at startup, then use reg+runtime clock??

    // output current date/time to serial port
    // TODO create string for output to serial port and writing to log file
    Serial.print(now.year(), DEC);
    Serial.print(F("/"));
    Serial.print(now.month(), DEC);
    Serial.print(F("/"));
    Serial.print(now.day(), DEC);
    Serial.print(F(" "));
    Serial.print(now.hour(), DEC);
    Serial.print(F(":"));
    Serial.print(now.minute(), DEC);
    Serial.print(F(":"));
    Serial.print(now.second(), DEC);
    Serial.print(F(","));
	
    // open logfile to write new sample record
    File logfile = sd.open("logfile.csv", FILE_WRITE);
    
    // write current date/time to logfile
    // TODO write 2 digit month/day (currently writes 1 digit if < 10)
    logfile.print(now.year(), DEC);
    logfile.print(F("/"));
    logfile.print(now.month(), DEC);
    logfile.print(F("/"));
    logfile.print(now.day(), DEC);
    logfile.print(F(" "));
    logfile.print(now.hour(), DEC);
    logfile.print(F(":"));
    logfile.print(now.minute(), DEC);
    logfile.print(F(":"));
    logfile.print(now.second(), DEC);
    logfile.print(F(","));

    // output sample index to serial port
    Serial.print(samples);
    Serial.print(F(","));

    // write sample index to log file
    //File logfile = sd.open("logfile.txt", FILE_WRITE);
    logfile.print(samples);
    logfile.print(F(","));
	
    // Read Sensor 1
    sensor.begin(1, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO why?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
		
      // output Sensor 1 sample to serial port
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.print(temperature);
      Serial.print(F(","));

      // write Sensor 1 sample to logfile
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.print(temperature);
      logfile.print(F(","));
    } else {
      // TODO write null fields if read failure
    }
    delay(200);                                 // sensors need delay before next read
      
    // Read Sensor 2
    sensor.begin(2, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO why?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
      
      // output Sensor 2 sample to serial port
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.print(temperature);
      Serial.print(F(","));

      // write Sensor 2 sample to logfile
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.print(temperature);
      logfile.print(F(","));
    } else {
      // TODO write null fields if read failure
    }
    delay(200);                                 // sensors need delay before next read
      
    // Read Sensor 3
    sensor.begin(3, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (TODO why?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
      
      // output Sensor 3 sample to serial port
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.println(temperature);               // end sample record with CRLF

      // write Sensor 3 sample to logfile
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.println(temperature);              // end sample record with CRLF
    } else {
      // TODO write null fields if read failure
    }

    // finished writing sample record, close logfile
    logfile.close();

    samples++;                                   // increment number of samples

  }
}
