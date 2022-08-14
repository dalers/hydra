/*
  Hydra Soil Moisture Sensor Logger

  (C) 2022 Dale Scott
  (C) 2018 Albertas Mickėnas (Miceuz)
*/

#include <Bounce2.h>          // https://www.arduino.cc/reference/en/libraries/bounce2/
#include <ModbusMaster.h>     // https://www.arduino.cc/reference/en/libraries/modbusmaster/
#include <SoftwareSerial.h>   // https://docs.arduino.cc/learn/built-in-libraries/software-serial
#include <SdFat.h>            // https://www.arduino.cc/reference/en/libraries/sdfat/
#include <LiquidCrystal.h>    // https://www.arduino.cc/reference/en/libraries/liquidcrystal/
#include <LCDKeypad.h>        // <https://osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield
#include "RTClib.h"           // https://www.arduino.cc/reference/en/libraries/rtclib/


// Initialize library objects
SoftwareSerial softSerial(2, 3); // RX, TX
Bounce button = Bounce();
ModbusMaster sensor;
SdFat sd;
LCDKeypad lcd;

#define DRIVER_ENABLE A1            // RS-485 transceiver direction

// SD Card
const int chipSelect = A2;

// ModbusMaster callback functions
// enable driver before sending
void preTransmission() {
  digitalWrite(DRIVER_ENABLE, HIGH);
}

// disable driver after sending
void postTransmission() {
  digitalWrite(DRIVER_ENABLE, LOW);
}

void setup() {
  Serial.begin(9600);               // Console
  softSerial.begin(19200);          // RS-485 (Moisture Sensor default)
  
  pinMode(A0, INPUT_PULLUP);        // config A0 for start button (RIGHT on LCD Keypad)

  pinMode(DRIVER_ENABLE, OUTPUT);

  button.attach(A0);                // attach bounce object to A0 (RIGHT)
  button.interval(5);               // debounce time 5ms

  sensor.begin(1, softSerial);                // setup comm with Modbus server Address 1
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF
  sensor.postTransmission(postTransmission);

  Serial.begin(9600);
  Serial.println(F("#Starting..."));

  if(sd.begin (chipSelect, SPI_HALF_SPEED)) {
    Serial.println(F("SD Card OK"));
  }

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(F("Hydra Controller"));
  lcd.setCursor(0,1);
  lcd.print(F("Version 0.0.2"));
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print(F("Press R to Start"));

}

void loop() {
  if(button.fell()) {   // if RIGHT has been pressed
    
      delay(100);

      uint8_t  tests = 0;           // clear previous test results
      uint16_t moisture = 0;
      int16_t  temperature = 0;
      uint16_t ver = 0;
      uint8_t  err = 0;

      while(tests < 10 && err == 0) { // read 10 times without error?
        uint8_t r = sensor.readInputRegisters(0, 3);
        
        if(0 != r) {
          Serial.print(F("#SENSOR Read Error: "));
          Serial.println(r);
          //digitalWrite(LED1, LOW);    // display error code on LED1/2
          //digitalWrite(LED2, HIGH);
          delay(100);
          err = 1;
        } else {
          moisture = sensor.getResponseBuffer(0);
          temperature = sensor.getResponseBuffer(1);
          ver = sensor.getResponseBuffer(2); // appears there are actually 3 input registers! (moisture, temperature and fw version)
          
          tests++; // increment number of tests
          
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
      
      while(LOW == button.read()){
        button.update();
      }
    delay(100);
    //digitalWrite(LED2, LOW);
    //digitalWrite(LED1, LOW);

  } else if(button.rose()){
    //digitalWrite(LED2, LOW);
    //digitalWrite(LED1, LOW);
  }
  
  button.update();

}
