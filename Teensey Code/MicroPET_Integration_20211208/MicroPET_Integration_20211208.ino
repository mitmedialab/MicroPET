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
#include <Adafruit_MCP23X17.h>

//Teensey 4.1 - SCL 19 - yellow /SDA 18 - blue 

//sensor


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

//IO ports

#define MCP_ADDR0 0x20  // IC5 on base board
#define MCP_ADDR1 0x21  // IC4 on base board
#define MCP_ADDR2 0x22  // IC1 on sensor board

#define LED 13

// Double check these pin assignments
#define GPA0 0
#define GPA1 1
#define GPA2 2
#define GPA3 3
#define GPA4 4
#define GPA5 5
#define GPA6 6
#define GPA7 7

#define GPB0 8
#define GPB1 9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15

#define experimentOne 1
#define experimentTwo 2

#define e_buffer 0
#define enzyme 1
#define chamberA 2
#define media 3
#define chamberB 4
#define waste 5
#define preservativeOne 6  
#define preservativeTwo 7
#define preservativeThree 8
#define preservativeFour 9
#define preservativeFive 10
#define preservativeSix 11


Adafruit_MCP23X17 mcp0;
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

typedef enum {
  // MCP0 - IC5 on base board
  BASEBD_J4_PIN2 = 0,
  BASEBD_J4_PIN4,
  BASEBD_J3_PIN2,
  BASEBD_J3_PIN4,
  BASEBD_J2_PIN2,
  BASEBD_J2_PIN4,
  BASEBD_J1_PIN2,
  BASEBD_J1_PIN4,
  BASEBD_J5_PIN2,
  BASEBD_J5_PIN4,
  BASEBD_J6_PIN2,
  BASEBD_J6_PIN4,
  BASEBD_J7_PIN2,
  BASEBD_J7_PIN4,
  BASEBD_J8_PIN2,
  BASEBD_J8_PIN4,

  // MCP1 - IC4 on base board
  BASEBD_J12_PIN2,
  BASEBD_J12_PIN4,
  BASEBD_J11_PIN2,
  BASEBD_J11_PIN4,
  BASEBD_J10_PIN2,
  BASEBD_J10_PIN4,
  BASEBD_J9_PIN2,
  BASEBD_J9_PIN4,
  
  //MCP2 - IC1 on sensor board
  SENSEBD_J2_PIN1,
  SENSEBD_J2_PIN2,
  SENSEBD_J2_PIN3,
  SENSEBD_J2_PIN4,
  SENSEBD_J3_PIN1,
  SENSEBD_J3_PIN2
} CONN_PIN_T; // Connector and Pin type

#define MCP1_PIN_OFFSET 16
#define MCP2_PIN_OFFSET 16

/****************************************/

boolean switch_conn_pin(CONN_PIN_T conn_pin, byte level)
{
  if((level != HIGH) && (level != LOW)) {
    return false;
  }
  
  if(conn_pin <= BASEBD_J8_PIN4) {
    mcp0.digitalWrite(conn_pin, level);
  } else if(conn_pin <= BASEBD_J9_PIN4) {
    mcp1.digitalWrite((conn_pin - MCP1_PIN_OFFSET), level);
  } else if(conn_pin <= SENSEBD_J3_PIN2) {
    mcp2.digitalWrite((conn_pin - MCP2_PIN_OFFSET), level);
  } else {
    return false;
  }

  return true;
}


String experimenta_log = "";


void setup() {
    init_card();
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(ledPin, OUTPUT);
    
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("Begin Serial"));

    pinMode(LED, OUTPUT);
// uncomment appropriate mcp.begin
  if (!mcp0.begin_I2C(MCP_ADDR0)) {
    Serial.println("Error initializing chip 0.");
    while (1);
  }

  if (!mcp1.begin_I2C(MCP_ADDR1)) {
    Serial.println("Error initializing chip 1.");
    while (1);
  }

  // IC5 on base board
  mcp0.pinMode(GPA0, OUTPUT);
  mcp0.pinMode(GPA1, OUTPUT);
  mcp0.pinMode(GPA2, OUTPUT);
  mcp0.pinMode(GPA3, OUTPUT);
  mcp0.pinMode(GPA4, OUTPUT);
  mcp0.pinMode(GPA5, OUTPUT);
  mcp0.pinMode(GPA6, OUTPUT);
  mcp0.pinMode(GPA7, OUTPUT);

  mcp0.pinMode(GPB0, OUTPUT);
  mcp0.pinMode(GPB1, OUTPUT);
  mcp0.pinMode(GPB2, OUTPUT);
  mcp0.pinMode(GPB3, OUTPUT);
  mcp0.pinMode(GPB4, OUTPUT);
  mcp0.pinMode(GPB5, OUTPUT);
  mcp0.pinMode(GPB6, OUTPUT);
  mcp0.pinMode(GPB7, OUTPUT);

  // IC4 on base board
  mcp1.pinMode(GPA0, OUTPUT);
  mcp1.pinMode(GPA1, OUTPUT);
  mcp1.pinMode(GPA2, OUTPUT);
  mcp1.pinMode(GPA3, OUTPUT);
  mcp1.pinMode(GPA4, OUTPUT);
  mcp1.pinMode(GPA5, OUTPUT);
  mcp1.pinMode(GPA6, OUTPUT);
  mcp1.pinMode(GPA7, OUTPUT);

/*****************************************************
 * Note - comment out below if only using base board
 
   if (!mcp2.begin_I2C(MCP_ADDR2)) {
    Serial.println("Error initializing chip 2.");
    while (1);
  }

  // IC1 on sensor board
  mcp2.pinMode(GPB0, OUTPUT);
  mcp2.pinMode(GPB1, OUTPUT);
  mcp2.pinMode(GPB2, OUTPUT);
  mcp2.pinMode(GPB3, OUTPUT);
  mcp2.pinMode(GPB4, OUTPUT);
  mcp2.pinMode(GPB5, OUTPUT);

  Serial.println("Looping...");
*/
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
    //DAY2
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,30,5), day_2); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY3
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,31,5), day_3); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY4
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,32,5), day_4); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY5
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,33,5), day_5); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY6
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,34,5), day_6); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY7
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,35,5), day_7); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY8
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,36,5), day_8); //  YYYY,  MM,  DD,  hh,  mm,  ss
    //DAY9
    Alarm.triggerOnce(tmConvert_t(2021,12,8,8,37,5), day_9); //  YYYY,  MM,  DD,  hh,  mm,  ss
   
  
    // EVERYDAY TASK
    Alarm.timerRepeat(60, Taking_Sensor_Data);   

    
}


void loop() { 
  digitalClockDisplay();
  Alarm.delay(1000);
}


void day_1(){ 
Serial.println("EXECUTING DAY_1 EXP");
/*####Day1 (Dec 9)
#Revival Chamber to Chamber B - ?ml (done manually)
#Start chamber A
#open waste valve
#enzyme to chamber A - 0.06 ml
#buffer to chamber A - 2.94 ml (flexible)
*/

Serial.println("start moving liquid");
switchCollection(experimentOne, waste);
moveLiquid (experimentOne, enzyme, chamberA, 75);
moveLiquid (experimentOne, e_buffer, chamberA, 2925);
Serial.println("finished moving liquid");
}

void day_2(){ 
Serial.println("EXECUTING DAY_2 EXP");
}

void day_3(){
Serial.println("EXECUTING DAY_3 EXP");
/*
#move chamber A to Chamber B - 2ml 
#move medium to chamber B - 2ml
#move chamber B to preservative A1(RNA) - 4ml
#move buffer to chamber A -  1.95ml
#move enzyme to chamber A - 0.05ml

*/

Serial.println("start moving liquid");
switchCollection(experimentOne, preservativeOne);
moveLiquid (experimentOne, chamberA, chamberB, 2000);
moveLiquid (experimentOne, media, chamberB, 2000);
moveLiquid (experimentOne, e_buffer, chamberA, 1950);
moveLiquid (experimentOne, enzyme, chamberA, 50);
Serial.println("finished moving liquid");

  }

void day_4(){ 
Serial.println("EXECUTING DAY_4 EXP");
}


void day_5(){
Serial.println("EXECUTING DAY_5 EXP");
/*
#move chamber A to chamber B - 2ml
#move medium to chamber B - 2ml
#move buffer to chamber A -  1.95ml
#move enzyme to chamber A - 0.05ml
#move chamber B to preservative A1(RNA) - 4ml
*/

Serial.println("start moving liquid");
switchCollection(experimentOne, preservativeOne);
moveLiquid (experimentOne, chamberA, chamberB, 2000);
moveLiquid (experimentOne, media, chamberB, 2000);
moveLiquid (experimentOne, e_buffer, chamberA, 1950);
moveLiquid (experimentOne, enzyme, chamberA, 50);
Serial.println("finished moving liquid");

  }
  
void day_6(){ 
Serial.println("EXECUTING DAY_6 EXP");
}

void day_7(){
Serial.println("EXECUTING DAY_7 EXP");

/*
#move chamber A to chamber B - 2ml
#move medium to chamber B - 2ml
#move buffer to chamber A -  1.95ml
#move enzyme to chamber A - 0.05ml
#move chamber B to preservative A1(RNA) - 4ml
*/

Serial.println("start moving liquid");
switchCollection(experimentOne, preservativeOne);
moveLiquid (experimentOne, chamberA, chamberB, 2000);
moveLiquid (experimentOne, media, chamberB, 2000);
moveLiquid (experimentOne, e_buffer, chamberA, 1950);
moveLiquid (experimentOne, enzyme, chamberA, 50);
Serial.println("finished moving liquid");

  }

void day_8(){ 
Serial.println("EXECUTING DAY_8 EXP");
}


void day_9(){
Serial.println("EXECUTING DAY_9 EXP");

/*
#move chamber A to chamber B - 2ml
#move medium to chamber B - 2ml
#move buffer to chamber A -  1.95ml
#move enzyme to chamber A - 0.05ml
#move chamber B to preservative A1(RNA) - 4ml
*/

Serial.println("start moving liquid");
switchCollection(experimentOne, preservativeOne);
moveLiquid (experimentOne, chamberA, chamberB, 2000);
moveLiquid (experimentOne, media, chamberB, 2000);
moveLiquid (experimentOne, e_buffer, chamberA, 1950);
moveLiquid (experimentOne, enzyme, chamberA, 50);
Serial.println("finished moving liquid");

  }



void moveLiquid(int experiment, int origin, int target, float liquid_volume) 
{
     if (experiment == 1 && origin == 0)
     {
      switch_conn_pin(BASEBD_J10_PIN2, LOW); //Experiment 1 valveA
      Serial.println("move from e_buffer");
      experimenta_log = experimenta_log + now() + "_move from e_buffer,";
      }

     if (experiment == 1 && origin == 1)
     {
      switch_conn_pin(BASEBD_J10_PIN2, HIGH); //Experiment 1 valveA
      Serial.println("move from enzyme");
      experimenta_log = experimenta_log + now() + "_move from enzyme,";
      }

     if (experiment == 1 && target == 2)
     {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
      Serial.println("move to chamberA");
      experimenta_log = experimenta_log + now() + "_move to chamberA,";
      for (int i = 0; i <= move_pulse; i++){
      switch_conn_pin(BASEBD_J10_PIN4, HIGH); //Experiment 1 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J10_PIN4, LOW); //Experiment 1 pumpA
      digitalWrite(LED, LOW);
      delay(200);
       Serial.println( i * 25); //print out the moving volume
      }
      switch_conn_pin(BASEBD_J10_PIN2, LOW); //Reset eperiment 1 valveA so enzyme is blocked
     }

     if (experiment == 1 && origin == 3) // media going out
     {
      switch_conn_pin(BASEBD_J9_PIN2, LOW); //Experiment 1 valveB
      Serial.println("move from media");
      experimenta_log = experimenta_log + now() + "_move from media,";
      }
     if (experiment == 1 && origin == 2) // chamberA going out
     {
      switch_conn_pin(BASEBD_J9_PIN2, HIGH); //Experiment 1 valveB
      Serial.println("move from chamberA");
      experimenta_log = experimenta_log + now() + "_move from chamberA,";
      }
     if (experiment == 1 && target == 4) 
     {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 35uL at a time
      Serial.println("move to chamberB");
      experimenta_log = experimenta_log + now() + "_move from chamberB,";
      for (int i = 0; i <= move_pulse; i++){
      switch_conn_pin(BASEBD_J9_PIN4, HIGH); //Experiment 1 pumpB
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J9_PIN4, LOW); //Experiment 1 pumpB
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25); //print out the moving volume
      } 
      switch_conn_pin(BASEBD_J9_PIN2, LOW); //Reset experiment 1 valveB so chamberA is blocked  
     }
}

void switchCollection(int experiment, int bag)
{
   if (experiment == 1 && bag == 5)
     {
      switch_conn_pin(BASEBD_J5_PIN4, LOW); //Experiment 1 waste
      Serial.println("open waste bag");
      experimenta_log = experimenta_log + now() + "_opem waste bag,";
      }
   if (experiment == 1 && bag == 6)
     {
      switch_conn_pin(BASEBD_J5_PIN4, HIGH); //Experiment 1 preservativeOne
      Serial.println("open preservativeOne bag");
      experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
      }
   
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
    myFile.println("DT, Temp, Pressure, Attitude, Humidity, Temp2, Spectral_Violet, Spectral_Blue, Spectral_Green, Spectral_Yellow, Spectral_Orange, Spectral_Red, Experimental_Log");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file
  
  
  
  }
