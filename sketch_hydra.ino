/*
  Hydra Soil Moisture Sensor Logger

  (C) 2022 Dale Scott
  (C) 2018 Albertas Mickėnas (Miceuz)
*/

// ***********************************
// LIBRARIES
// ***********************************

// serial library             // (built-in) https://www.arduino.cc/reference/en/language/functions/communication/serial/

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

SoftwareSerial softSerial(2, 3);    // RX, TX
Bounce button = Bounce();
ModbusMaster sensor;
SdFat sd;
LCDKeypad lcd;
RTC_PCF8523 rtc;
bool logging = false;

//Sd2Card card;
//SdVolume volume;
//SdFile root;

// SD Card
const int chipSelect = A2;          // TODO change to #define (same as DRIVER_ENABLE)

// ModbusMaster callback functions
// enable driver before sending
void preTransmission() {
  digitalWrite(DRIVER_ENABLE, HIGH);
}

// disable driver after sending
void postTransmission() {
  digitalWrite(DRIVER_ENABLE, LOW);
}

// Logging heartbeat (LCD blinks "*" top right when logging)
int logBlink = LOW;             // logBlink character state
long interval = 1000;           // logBlink interval (milliseconds)
long previousMillis = 0;        // time that logBlink was last updated


// ***********************************
// SETUP
// ***********************************

void setup() {
  Serial.begin(9600);               // Console
  softSerial.begin(19200);          // RS-485 (Moisture Sensors)
  
  pinMode(A0, INPUT_PULLUP);        // config A0 for start button (RIGHT on LCD Keypad)

  pinMode(DRIVER_ENABLE, OUTPUT);

  button.attach(A0);                // attach bounce object to A0 (RIGHT)
  button.interval(5);               // debounce time 5ms

  Serial.begin(9600);
  Serial.println(F("#Starting"));

  // show beginning of startup on LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(F("Hydra Controller"));
  lcd.setCursor(0,1);
  lcd.print(F("Starting..."));
  delay(1000);
  
  // SD Card
  if(sd.begin (chipSelect, SPI_HALF_SPEED)) {
    Serial.println(F("#SD Card found"));
    //TODO output file names and remaining space on SD Card
  } else {
    Serial.println(F("#SD Card NOT found"));
    // TODO set flag if SD Card not found
  }

  // RTC
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

  // Modbus
  
  uint8_t r;	// Modbus read status
  
  // Moisture Sensor 1
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
  delay(1000);
    
  // Moisture Sensor 2
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
  delay(1000);
	  
  // Moisture Sensor 3
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
  delay(1000);
	  
  // show startup complete on LCD
  lcd.setCursor(0,1);
  lcd.print(F("Hydra v0.0.2"));
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print(F("Start Log - R"));

}

// ***********************************
// MAIN LOOP
// ***********************************

void loop() {

  // TODO handle if SD Card, RTC or sensors not found
  
  button.update();
  if(button.fell()) {             // if RIGHT was pressed and released

    logging = true;               // TODO remove if not used
    
    delay(100);

    uint8_t  samples = 0;         // sample counter
    uint16_t moisture = 0;        // sensor moisture value
    int16_t  temperature = 0;     // sensor temperature value
    uint8_t  r = 0;               // sensor return value

    uint16_t ver = 0;             // TODO delete if not used
    uint8_t  err = 0;             // TODO delete if not used
    
    // Sample Sensor 1
    sensor.begin(1, softSerial);
    sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF (is this needed?)
    sensor.postTransmission(postTransmission);

    r = sensor.readInputRegisters(0, 2);
    if (0 == r) {
      moisture = sensor.getResponseBuffer(0);
      temperature = sensor.getResponseBuffer(1);
	  
	  Serial.print(F("#Sensor 1 Moisture: "));
      Serial.print(sensor.getResponseBuffer(0));
      Serial.print(F(", Temp: "));
      Serial.println(sensor.getResponseBuffer(1));

      // TODO write all three sensors at once
      // TODO generate filename based on date and time of day e.g. ddhhmmss.log KISS
      File results = sd.open("results.txt", FILE_WRITE);
      results.print(moisture);
      results.print(",");
      results.print(temperature);
      results.close();
    } else {
    // TODO handle error if read failure
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
	  
	  Serial.print(F("#Sensor 2 Moisture: "));
      Serial.print(sensor.getResponseBuffer(0));
      Serial.print(F(", Temp: "));
      Serial.println(sensor.getResponseBuffer(1));

      File results = sd.open("results.txt", FILE_WRITE);
      results.print(moisture);
      results.print(",");
      results.print(temperature);
      results.close();
    } else {
    // TODO handle error if read failure
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
	  
	  Serial.print(F("#Sensor 3 Moisture: "));
      Serial.print(sensor.getResponseBuffer(0));
      Serial.print(F(", Temp: "));
      Serial.println(sensor.getResponseBuffer(1));

      File results = sd.open("results.txt", FILE_WRITE);
      results.print(moisture);
      results.print(",");
      results.println(temperature);              // add CRLF after Sensor 3
      results.close();
    } else {
    // TODO handle error if read failure
    }
    delay(1000);

	  
    samples++;                                  // TODO show samples on LCD

    delay(60000);                                // delay 1m = 60,000sec

    // TODO replace delay with timer and add blinking logging LED

#if 0
      while(samples < 10 && err == 0) { // read 10 times without error?
        uint8_t r = sensor.readInputRegisters(0, 3);
        
        if(0 != r) {
          Serial.print(F("#SENSOR Read Error: "));
          Serial.println(r);
          delay(100);
          err = 1;
        } else {
          moisture = sensor.getResponseBuffer(0);
          temperature = sensor.getResponseBuffer(1);
          ver = sensor.getResponseBuffer(2); // appears there are actually 3 input registers! (moisture, temperature and fw version)
          
          samples++; // increment number of samples
          
          if(moisture < 100 || moisture > 600) {
            Serial.println(moisture);
            Serial.println(F("#SENSOR Moisture Out Of Bounds"));
            err = 1;
          }
  
          if(temperature < 150 || temperature > 350) {
            Serial.println(temperature);
            Serial.println(F("#SENSOR Temperature Out Of Bounds"));
            err = 1;
          }
  
          // deal with error
          if(err) {
             // handle error
          } else {
             // all ok
          }
        }
      }
      
      if(err == 0){
        Serial.print(moisture);
        Serial.print(", ");
        Serial.println(temperature);
        File results = sd.open("results.txt", FILE_WRITE);
        results.print(moisture);
        results.print(",");
        results.println(temperature);
        results.close();
      }
#endif

  }
}
