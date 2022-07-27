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


// Initialize objects
SoftwareSerial softSerial(2, 3); // RX, TX
Bounce button = Bounce();
ModbusMaster sensor;
SdFat sd;
LCDKeypad lcd;

// RS-485 transceiver
#define DRIVER_ENABLE A1

// Sensor power control
//#define SENSOR_PWR 8       // sensor power not controlled

// LED1 and LED2 show test status
//#define LED1 6             // LED1 on SD Data Logger not wired
//#define LED2 A1            // LED2 on SD Data Logger not wired

// SD Card
const int chipSelect = A2;

// RS-485 transceiver direction control
// enable driver before sending
void preTransmission() {
  digitalWrite(DRIVER_ENABLE, HIGH);
}

// disable driver after sending
void postTransmission() {
  digitalWrite(DRIVER_ENABLE, LOW);
}

uint16_t data[6];                   // Modbus receive data buffer?? **DOES NOT APPEAR TO BE USED**


void setup() {
  Serial.begin(9600);               // Console
  softSerial.begin(19200);          // RS-485 (Moisture Sensor default)
  
  pinMode(A0, INPUT_PULLUP);        // config A0 for start button (RIGHT on LCD Keypad)

  // LED1 and LED2 (SD Data Logger shield) not wired
  //pinMode(LED1, OUTPUT);
  //pinMode(LED2, OUTPUT);

  pinMode(DRIVER_ENABLE, OUTPUT);

//  pinMode(SENSOR_PWR, OUTPUT);    // sensor power is not controlled
//  digitalWrite(SENSOR_PWR, HIGH);
  
  button.attach(A0);                // attach bounce object to A0/RIGHT
  button.interval(5);               // debounce time 5ms

  // blink LED1 and LED2 at startup
//  digitalWrite(LED1, HIGH);
//  digitalWrite(LED2, HIGH);
//  delay(1000);
//  digitalWrite(LED1, LOW);
//  digitalWrite(LED2, LOW);

  sensor.begin(1, softSerial);                // communicate with Modbus slave ID 1 over softSerial
  sensor.preTransmission(preTransmission);    // toggle RS-485 driver ON/OFF
  sensor.postTransmission(postTransmission);

  Serial.begin(9600);
  Serial.println("#Begins...");

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Hydra Controller");
  lcd.setCursor(0,1);
  lcd.print("Version 0.0.2");

  if(sd.begin (chipSelect, SPI_HALF_SPEED)) {
    Serial.println("SD Card OK");
  }
}

void loop() {
  if(button.fell()) {   // if RIGHT has been pressed
    
      //digitalWrite(LED1, LOW);    // reset LED1 and LED2 to Off
      //digitalWrite(LED2, LOW);
      delay(100);

      uint8_t  tests = 0;           // clear previous test results
      uint16_t moisture = 0;
      int16_t  temperature = 0;
      uint16_t ver = 0;
      uint8_t  err = 0;

      while(tests < 10 && err == 0) { // read 10 times without error?
        uint8_t r = sensor.readInputRegisters(0, 3);
        
        if(0 != r) {
          Serial.print("#SENSOR Read Error: ");
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
            Serial.println("#SENSOR Moisture Out Of Bounds");
            err = 1;
          }
  
          if(temperature < 150 || temperature > 350) {
            Serial.println(temperature);
            Serial.println("#SENSOR Temperature Out Of Bounds");
            err = 1;
          }
  
          if(err) {
            //digitalWrite(LED1, LOW);      // display error code on LED1/LED2
            //digitalWrite(LED2, HIGH);
            delay(200);
          } else {
            //digitalWrite(LED1, HIGH);
            //digitalWrite(LED2, LOW);
            delay(50);
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
