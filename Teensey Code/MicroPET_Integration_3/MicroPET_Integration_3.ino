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
#include <TimeLib.h>
#include <TimeAlarms.h>
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


//SD Card
#include <SD.h>
#include <SPI.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;
const int ledPin = 5;




void setup() {
    init_card();
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(ledPin, OUTPUT);
    
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
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();



    // EXPERIMENTAL SETUP
      
    //STARTING DATE
    setTime(8,29,0,8,12,2021);  // hr,min,sec,day,mnth,yr

    //DAY1
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,29,5), day_1); //  YYYY,  MM,  DD,  hh,  mm,  ss
  

    // EVERYDAY TASK
    Alarm.timerRepeat(10, Taking_Sensor_Data);   

    
}


void loop() { 
  digitalClockDisplay();
  Alarm.delay(1000);
}


void day_1(){
  Serial.println("Experimental_date");    
}

void Taking_Sensor_Data(){
  Serial.println("Taking Sensor Data");
  get_data();    
}


void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(now());
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet); 
}




void get_data() {
  //AMS
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


          
 //SAVING
                // open the file. 
            myFile = SD.open("savefile.csv", FILE_WRITE);
                if (myFile) 
                    {
                    Serial.print("Writing to file...");
                    //myFile.println();

                    //DT
                    myFile.print(now());
                    myFile.print(',');


                    //Temp
                    myFile.print(bme.readTemperature());
                    myFile.print(',');

                    //Pressure
                    myFile.print(bme.readPressure() / 100.0F);
                    myFile.print(',');

                    //Attitude
                    myFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
                    myFile.print(',');

                    //Humidity
                    myFile.print(bme.readHumidity());
                    myFile.print(',');

                    //Temp2
                    myFile.print(temp);
                    myFile.print(',');

                    //Spectral_Violet
                    myFile.print(sensorValues[AS726x_VIOLET]);
                    myFile.print(',');

                    //Spectral_Blue
                    myFile.print(sensorValues[AS726x_VIOLET]);
                    myFile.print(',');

                    //Spectral_Green
                    myFile.print(sensorValues[AS726x_GREEN]);
                    myFile.print(',');
                    
                    //Spectral_Yellow
                    myFile.print(sensorValues[AS726x_YELLOW]);
                    myFile.print(',');
                    
                    //Spectral_Orange
                    myFile.print(sensorValues[AS726x_ORANGE]);
                    myFile.print(',');
                    
                    //Spectral_Red
                    myFile.print(sensorValues[AS726x_RED]);

                    
                    myFile.println();
                    myFile.close();
                    Serial.println("done.");
                    digitalWrite(ledPin, HIGH);   // set the LED on
                    delay(300);                  // wait for a second
                    digitalWrite(ledPin, LOW);    // set the LED off
                    delay(300);                  // wait for a second
                    } 
              else {
                    Serial.println("error opening file");
              }
  
}

void init_card(){
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  // open the file. 
  myFile = SD.open("savefile.csv", FILE_WRITE);
  if (myFile) {
    Serial.print("Init header to file...");
    digitalWrite(ledPin, HIGH);   // set the LED on
    delay(1000);                  // wait for a second
    digitalWrite(ledPin, LOW);    // set the LED off
    delay(1000);                  // wait for a second

    //Content
    myFile.println("DT, Temp, Pressure, Attitude, Humidity, Temp2, Spectral_Violet, Spectral_Blue, Spectral_Green, Spectral_Yellow, Spectral_Orange, Spectral_Red");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file
  
  
  
  }
