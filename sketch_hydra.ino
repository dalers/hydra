/*
  Hydra Soil Moisture Sensor Data Logger
  (C) 2023 Dale Scott
  Provided according to the terms of the GPL v3

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
uint8_t samples = 1;                // sample counter
uint8_t r;	                        // Modbus read status

SoftwareSerial softSerial(2, 3);    // RX, TX
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

// TODO implement logging heartbeat in LCD when logging (e.g. blink "*" top right)
int logBlink = LOW;             // logBlink character state
long interval = 1000;           // logBlink interval (milliseconds)
long previousMillis = 0;        // time that logBlink was last updated

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
  lcd.clear(); // sets cursor to (col 0, row 0)
  lcd.print(F("Hydra Controller"));
  lcd.setCursor(0,1);
  lcd.print(F("FW 0.0.3"));
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
      Serial.print(F("-"));
      Serial.print(now.month(), DEC);
      Serial.print(F("-"));
      Serial.print(now.day(), DEC);
      Serial.print(F("T"));
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
    //TODO output file names and remaining space on SD Card
  } else {
    Serial.println(F("#SD Card NOT found"));
    // TODO set flag if SD Card not found
  }

  // Probe Moisture Sensors
  // Sensor 1
  sensor.begin(1, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
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
  delay(1000);                                 // wait 1sec before next poll
    
  // Sensor 2
  sensor.begin(2, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
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
  delay(1000);                                 // wait 1sec before next poll
	  
  // Sensor 3
  sensor.begin(3, softSerial);
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
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
  delay(1000);                                 // wait 1sec before next poll TODO is this needed since next poll is a ways away?
	  
  // startup complete, show logging prompt
  lcd.setCursor(0,1);
  lcd.print(F("Start Logging >R"));

  Serial.println(F("#Startup Complete"));

}

// ***********************************
// MAIN LOOP
// ***********************************

void loop() {

  // TODO handle if probe of RTC, SD Card, or sensors fails
  
  button.update();
  if(button.fell()) {             // if RIGHT was pressed and released
    Serial.println(F("#RIGHT pressed"));
  
    logging = !(logging);
    Serial.print(F("#Logging state: ")); Serial.println(logging);

    if (logging == true) {
      lcd.setCursor(0,1);
      lcd.print(F("Stop Logging  >R"));
	  // TODO generate log filename based on current date/time e.g. "ddhhmmss.log"
    } else {
      lcd.setCursor(0,1);
      lcd.print(F("Start Logging >R"));
    }
  }
    
  uint16_t moisture = 0;        // sensor moisture value
  int16_t  temperature = 0;     // sensor temperature value
  uint8_t  r = 0;               // sensor return value

  uint8_t  err = 0;             // read error counter (per sample)

  if (logging) {

    DateTime now = rtc.now();   // get current date/time

    // output current date/time to serial port
    // TODO create string for output to serial port and writing to log file
    // - write 2 digit month/day, currently writes 1 digit if < 10!
    Serial.print(now.year(), DEC);
    Serial.print(F("-"));
    Serial.print(now.month(), DEC);
    Serial.print(F("-"));
    Serial.print(now.day(), DEC);
    Serial.print(F("T"));
    Serial.print(now.hour(), DEC);
    Serial.print(F(":"));
    Serial.print(now.minute(), DEC);
    Serial.print(F(":"));
    Serial.print(now.second(), DEC);
    Serial.print(F(","));
	
    // open logfile
    File logfile = sd.open("logfile.txt", FILE_WRITE);
    
    // write current date/time to logfile
    // TODO create string for output to serial port and writing to log file
    // - write 2 digit month/day, currently writes 1 digit if < 10!
    //File logfile = sd.open("logfile.txt", FILE_WRITE);
    logfile.print(now.year(), DEC);
    logfile.print(F("-"));
    logfile.print(now.month(), DEC);
    logfile.print(F("-"));
    logfile.print(now.day(), DEC);
    logfile.print(F("T"));
    logfile.print(now.hour(), DEC);
    logfile.print(F(":"));
    logfile.print(now.minute(), DEC);
    logfile.print(F(":"));
    logfile.print(now.second(), DEC);
    logfile.print(F(","));
    //logfile.close();

    // output sample index to serial port
    Serial.print(samples);
    Serial.print(F(","));

    // write sample index to log file
    //File logfile = sd.open("logfile.txt", FILE_WRITE);
    logfile.print(samples);
    logfile.print(F(","));
    //logfile.close();
	
    // Sample Sensor 1
    sensor.begin(1, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
    sensor.postTransmission(postTransmission);
    delay(1000);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
		
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.print(temperature);
      Serial.print(F(","));

      // TODO generate filename based on date and time of day e.g. ddhhmmss.log KISS
      //File logfile = sd.open("logfile.txt", FILE_WRITE);
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.print(temperature);
      logfile.print(F(","));
      //logfile.close();
    } else {
      // TODO write commas for null fields
    }
    delay(1000);
      
    // Sample Sensor 2
    sensor.begin(2, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
      
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.print(temperature);
      Serial.print(F(","));

      // TODO use string variable for filename
      //File logfile = sd.open("logfile.txt", FILE_WRITE);
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.print(temperature);
      logfile.print(F(","));
      //logfile.close();
    } else {
      // TODO write commas for null fields
    }
    delay(1000);
      
    // Sample Sensor 3
    sensor.begin(3, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
      
      Serial.print(moisture);
      Serial.print(F(","));
      Serial.println(temperature);               // end of sample (3 sensors)

      // TODO generate filename based on date and time of day e.g. ddhhmmss.log KISS
      //File logfile = sd.open("logfile.txt", FILE_WRITE);
      logfile.print(moisture);
      logfile.print(F(","));
      logfile.println(temperature);              // end of sample (3 sensors)
      //logfile.close();
    } else {
      // TODO write commas for null fields if read failure
    }
    //delay(1000);                               // no delay needed due to delay before next sample

    samples++;                                   // increment number of samples

    // close logfile
    logfile.close();

    // TODO blink LCD logging indicator

    // wait until time for next sample           // TODO replace with timer
    delay(60000);                                // 5000=5sec, 60000=1min, 300000=5min
  }
}
