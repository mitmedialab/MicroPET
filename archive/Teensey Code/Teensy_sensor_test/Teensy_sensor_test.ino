/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_AS726x.h>

//Teensey 4.1 - SCL 19 - yellow /SDA 18 - blue 


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_AS726x ams;
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
uint16_t sensorValues[AS726x_NUM_CHANNELS];


void setup() {
    delayTime = 6000;
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("Begin Serial"));

    unsigned bme_status;
    unsigned ams_status;
    // default settings
    bme_status = bme.begin();  
    ams_status = ams.begin();  
    // You can also pass in a Wire library object like &Wire2

    if(!ams_status){
        Serial.println("could not connect to sensor! Please check your wiring.");
        while(1); delay(10);
    }
    
    if (!bme_status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        while (1) delay(10);
    }
    
    //Serial.println("-- Default Test --");
    delayTime = 1000;

    //Serial.println();
}


void loop() { 
    unsigned bme_status;
    unsigned ams_status;
    bme_status = bme.begin();  
    ams_status = ams.begin();  
    // You can also pass in a Wire library object like &Wire2

    if(!ams_status){
        Serial.println("could not connect to sensor! Please check your wiring.");
        while(1); delay(10);
    }
    
    if (!bme_status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        while (1) delay(10);
    }
    
    //Serial.println("-- Default Test --");
    delayTime = 1000;


  
  AMSprintValues();
  BMEprintValues();
  analogWrite(3, 200);  // LED on
  analogWrite(4, 0); // LED off  xx .  8
  delay(delayTime);
  
  analogWrite(3, 0);  // LED on
  analogWrite(4, 0); // LED off
  delay (50);
 
}

void AMSprintValues() {
  //read the device temperature
  uint8_t temp = ams.readTemperature();
  
  ams.drvOn(); //uncomment this if you want to use the driver LED for readings
  ams.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    delay(5);
    rdy = ams.dataReady();
  }
  //ams.drvOff(); //uncomment this if you want to use the driver LED for readings

  //read the values!
  ams.readRawValues(sensorValues);
  //ams.readCalibratedValues(calibratedValues);

  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" Violet: "); Serial.print(sensorValues[AS726x_VIOLET]);
  Serial.print(" Blue: "); Serial.print(sensorValues[AS726x_BLUE]);
  Serial.print(" Green: "); Serial.print(sensorValues[AS726x_GREEN]);
  Serial.print(" Yellow: "); Serial.print(sensorValues[AS726x_YELLOW]);
  Serial.print(" Orange: "); Serial.print(sensorValues[AS726x_ORANGE]);
  Serial.print(" Red: "); Serial.print(sensorValues[AS726x_RED]);
  Serial.println();
}

  
void BMEprintValues() {

    //BME
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

}
