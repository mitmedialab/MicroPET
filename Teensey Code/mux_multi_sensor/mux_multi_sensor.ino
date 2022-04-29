#include <Wire.h>              // generic I2C lib
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_AS726x.h>

// Native I2C address of the mux
#define MUX_ADDR    0x70
// Native I2C address of both AS7262 sensors
#define AS7262_ADDR 0x49
// Native I2C address of both BME280 sensors
#define BME280_ADDR 0x76
// Sensor positions on the mux
#define AS7262_SENSOR1  3
#define AS7262_SENSOR2  0
#define BME280_SENSOR1  2
#define BME280_SENSOR2  1

Adafruit_BME280 bme;
Adafruit_AS726x ams;

#define SEALEVELPRESSURE_HPA (1013.25)
uint16_t sensorValues[AS726x_NUM_CHANNELS];

//SD Card
#include <SD.h>
#include <SPI.h>

typedef struct{
  byte experimentStarted;
  time_t epoch;
  byte testDay;
  byte testInterval;
}systemStateStruct;

File myFile;
const int chipSelect = BUILTIN_SDCARD;
const int ledPin = 5;

systemStateStruct systemStateStructVar = {0,0,0,0};

String experimenta_log = "";
unsigned int whichsensor = 0;

// Function to select a sensor
void tcaselect(uint8_t sensor_addr) {
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << sensor_addr);
  Wire.endTransmission();
}

void setup() {
  // put your setup code here, to run once:
  //unsigned status;
  Serial.begin(9600);
  while(!Serial);    // time to get serial running
  Serial.println(F("Begin Serial"));
  
  init_card();
 // default settings
  unsigned bme_status;
  unsigned ams_status; 

  //Start the sensors line 1
 tcaselect(BME280_SENSOR1);
  delay(1000);
  // start the sensor
  bme_status = bme.begin();  
  ams_status = ams.begin(); 
  delay(1000); 

 if(!ams_status){
        Serial.println("could not connect to AS726x sensor1, Please check your wiring.");
     //   while(1); delay(10);
    }
    
if (!bme_status) {
        Serial.println("Could not find a valid BME280 sensor1, check wiring, address, sensor ID!");
     //   while (1) delay(10);
    }

  //Start the sensors line　2
  tcaselect(BME280_SENSOR1);
  delay(1000);
  // start the sensor
  bme_status = bme.begin();  
  ams_status = ams.begin(); 
  delay(1000); 
  
if(!ams_status){
        Serial.println("could not connect to AS726x sensor2, Please check your wiring.");
     //   while(1); delay(10);
    }
    
if (!bme_status) {
        Serial.println("Could not find a valid BME280 sensor2, check wiring, address, sensor ID!");
     //   while (1) delay(10);
    }
    
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp1, temp2;
  uint16_t as1_sensorValues[AS726x_NUM_CHANNELS], as2_sensorValues[AS726x_NUM_CHANNELS];

// Select the first BME280 and AS726xread some data
  tcaselect(BME280_SENSOR1);
  Serial.println("Sensor Data Experiment One");
  whichsensor = 1;
  delay(100);
  get_data();
  delay(2000);
// Select the second BME280 and AS726xread some data
  tcaselect(BME280_SENSOR2);
  Serial.println("Sensor Data Experiment Two");
  whichsensor = 2;
  delay(100);
  get_data();
  delay(2000);
}

void get_data() {


  //AMS
  //read the device temperature
  uint8_t temp = ams.readTemperature();

  ams.drvOn(); //uncomment this if you want to use the driver LED for readings
  ams.startMeasurement(); //begin a measurement

  //wait till data is available
  bool rdy = false;
  while (!rdy) {
    delay(5);
    rdy = ams.dataReady();
  }

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

  ams.drvOff();

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
                 //   myFile.print(now());
                    myFile.print("placeholder for current time"); 
                    myFile.print(',');

                    //millis
                    myFile.print(millis());
                    myFile.print(',');

                    //whichsensor
                    myFile.print(whichsensor);
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
                    myFile.print(',');

                    //EXP Log
                    myFile.print(experimenta_log);
                    
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
    myFile.println("DT, millis,Sensorline, Temp, Pressure, Attitude, Humidity, Temp2, Spectral_Violet, Spectral_Blue, Spectral_Green, Spectral_Yellow, Spectral_Orange, Spectral_Red, Experimental_Log");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file
  
  
  
  }
