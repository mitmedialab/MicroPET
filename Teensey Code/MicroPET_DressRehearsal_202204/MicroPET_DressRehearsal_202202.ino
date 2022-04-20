/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.



  Base Board Connection:
  E1VA + E1PA - J10
  E1VB + E1PB - J09
  E2VA + E2PA - J12
  E2VB + E2PB - J11
  the valve board connector J1-6 are the same on the base board

 ***************************************************************************/
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_AS726x.h>
#include <Adafruit_MCP23X17.h>
#include "RTClib.h"

<<<<<<< Updated upstream
/* Experiment Prototypes and supporting defines */
#define TOTAL_EXPERIMENTS 8
void day_0();
void day_1();
void day_2();
void day_3();
void day_4();
void day_5();
void day_6();
void day_7();
void *experimentArray[TOTAL_EXPERIMENTS] = {day_0, day_1, day_2, day_3, day_4, day_5, day_6, day_7};
=======
//Teensey 4.1 - SCL 19 - yellow /SDA 18 - blue
>>>>>>> Stashed changes

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

RTC_PCF8523 rtc;

Adafruit_AS726x ams;
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
uint16_t sensorValues[AS726x_NUM_CHANNELS];

//SD Card
#include <SD.h>
#include <SPI.h>

typedef struct {
  byte experimentStarted;
  time_t epoch;
  byte testDay;
  byte testDayComplete;
  byte testInterval;
} systemStateStruct;

File myFile;
const int chipSelect = BUILTIN_SDCARD;
const int ledPin = 5;

systemStateStruct systemStateStructVar = {0, 0, 0, 0};

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

#define waste 0
#define preservativeOne 1
#define preservativeTwo 2
#define preservativeThree 3
#define preservativeFour 4
#define preservativeFive 5


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
  if ((level != HIGH) && (level != LOW)) {
    return false;
  }

  if (conn_pin <= BASEBD_J8_PIN4) {
    mcp0.digitalWrite(conn_pin, level);
  } else if (conn_pin <= BASEBD_J9_PIN4) {
    mcp1.digitalWrite((conn_pin - MCP1_PIN_OFFSET), level);
  } else if (conn_pin <= SENSEBD_J3_PIN2) {
    mcp2.digitalWrite((conn_pin - MCP2_PIN_OFFSET), level);
  } else {
    return false;
  }

  return true;
}


String experimenta_log = "";

#define SECONDS_IN_DAY 86400
//#define DELAY_SECONDS_PER_EXPERIMENT SECONDS_IN_DAY
#define DELAY_SECONDS_PER_EXPERIMENT 600 // 10 mins for the functional testing
#define SERIAL_TIMEOUT 10000 //milliseconds

const unsigned int MAX_MESSAGE_LENGTH = 12;
bool systemState = 0;

AlarmID_t Alarm_Day1_ID;
AlarmID_t Alarm_Day2_ID;
AlarmID_t Alarm_Day3_ID;
AlarmID_t Alarm_Day4_ID;
AlarmID_t Alarm_Day5_ID;
AlarmID_t Alarm_Day6_ID;
AlarmID_t Alarm_Day7_ID;
AlarmID_t Alarm_Day8_ID;
AlarmID_t Alarm_Day9_ID;


void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  long unsigned startup_time = millis();
  // the below code waits for a serial monitor session to be opened
  // but times out in SERIAL_TIMEOUT milliseconds if none exist
  while (!Serial) {
    // timeout check
    if ( (millis() - startup_time) > SERIAL_TIMEOUT) {
      break;
    }
  }

  Serial.println(F("Begin Serial"));

  init_card();
  rtcInit();

  pinMode(LED, OUTPUT);
  // uncomment appropriate mcp.begin
  if (!mcp0.begin_I2C(MCP_ADDR0)) {
    Serial.println("Error initializing chip 0.");
    // while (1);
  }

  if (!mcp1.begin_I2C(MCP_ADDR1)) {
    Serial.println("Error initializing chip 1.");
    // while (1);
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
     Note - comment out below if only using base board

<<<<<<< Updated upstream
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
     Serial.println("no sensors in this functional test");
    if(!ams_status){
        Serial.println("could not connect to sensor! Please check your wiring.");
     //   while(1); delay(10);
    }
    
    if (!bme_status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
     //   while (1) delay(10);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

    // EXPERIMENTAL SETUP
      
    //STARTING DATE
//    setTime(11,0,0,12,2,2022);  // hr,min,sec,day,mnth,yr
    Serial.println("Setting time from RTC");
    setTime(rtc.now().unixtime()); //sets time from RTC
    Serial.print("Time is now: ");
    Serial.println(now());

    if(!grabStateFromSD(&systemStateStructVar)){
        Serial.println("No log file exists so will create one!");
        // no prior state exists so saving a new file with current rtc time
        systemStateStructVar.experimentStarted = 0;
        systemStateStructVar.epoch = rtc.now().unixtime();
        saveStateToSD(&systemStateStructVar);
    }
    else if(systemStateStructVar.experimentStarted==0){
      Serial.println("Previous log file exists but experiment hasn't started!");
      // if a file exists and shows no prior experiment has started, update time
        systemStateStructVar.experimentStarted = 0;
        systemStateStructVar.epoch = rtc.now().unixtime();
        saveStateToSD(&systemStateStructVar);
    }
    else if (systemStateStructVar.experimentStarted==1){
      Serial.println("Power Failure Event Detected!");
      // log file has shown that experiment has started already
      // todo: implement schedule correction
      systemState = true;
      recoverSystemStart();
=======
     if (!mcp2.begin_I2C(MCP_ADDR2)) {
      Serial.println("Error initializing chip 2.");
      while (1);
>>>>>>> Stashed changes
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
  Serial.println("no sensors in this functional test");
  if (!ams_status) {
    Serial.println("could not connect to sensor! Please check your wiring.");
    //   while(1); delay(10);
  }

  if (!bme_status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    //   while (1) delay(10);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();

  // EXPERIMENTAL SETUP

  //STARTING DATE
  //    setTime(11,0,0,12,2,2022);  // hr,min,sec,day,mnth,yr
  Serial.println("Setting time from RTC");
  setTime(rtc.now().unixtime()); //sets time from RTC
  Serial.print("Time is now: ");
  Serial.println(now());

  if (!grabStateFromSD(&systemStateStructVar)) {
    Serial.println("No log file exists so will create one!");
    // no prior state exists so saving a new file with current rtc time
    systemStateStructVar.experimentStarted = 0;
    systemStateStructVar.epoch = rtc.now().unixtime();
    saveStateToSD(&systemStateStructVar);
  }
  else if (systemStateStructVar.experimentStarted == 0) {
    Serial.println("Previous log file exists but experiment hasn't started!");
    // if a file exists and shows no prior experiment has started, update time
    systemStateStructVar.experimentStarted = 0;
    systemStateStructVar.epoch = rtc.now().unixtime();
    saveStateToSD(&systemStateStructVar);
  }
  else if (systemStateStructVar.experimentStarted == 1) {
    Serial.println("Power Failure Event Detected!");
    // log file has shown that experiment has started already
    // todo: implement schedule correction
  }

  //    //DAY1
  //    Alarm_Day1_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,1,5), day_1); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY2
  //    Alarm_Day2_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,3,5), day_2); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY3
  //    Alarm_Day3_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,5,5), day_3); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY4
  //    Alarm_Day4_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,7,5), day_4); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY5
  //    Alarm_Day5_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,9,5), day_5); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY6
  //    Alarm_Day6_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,11,5), day_6); //  YYYY,  MM,  DD,  hh,  mm,  ss
  //    //DAY7
  //    Alarm_Day7_ID = Alarm.triggerOnce(tmConvert_t(2022,2,18,11,13,5), day_7); //  YYYY,  MM,  DD,  hh,  mm,  ss

  // EVERYDAY TASK
  Alarm.timerRepeat(60, Taking_Sensor_Data);
  //day_0(); //debug mode

}


void loop() {
  //  digitalClockDisplay();
  Alarm.delay(1000);

  // if system has not already started, check for start condition via serial
  if (!systemState && checkForStartSerialCommand()) {

    // system has been commanded to start
    forceSystemStart();

    // set system state
    systemState = true;
  }
}

void rtcInit() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }
  else {
    Serial.println("Found RTC");

    // following line sets the RTC to the date & time this sketch was compiled
    // Uncomment it when setting up the first time/change battery
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //
    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
  }

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();

  // The PCF8523 can be calibrated for:
  //        - Aging adjustment
  //        - Temperature compensation
  //        - Accuracy tuning
  // The offset mode to use, once every two hours or once every minute.
  // The offset Offset value from -64 to +63. See the Application Note for calculation of offset values.
  // https://www.nxp.com/docs/en/application-note/AN11247.pdf
  // The deviation in parts per million can be calculated over a period of observation. Both the drift (which can be negative)
  // and the observation period must be in seconds. For accuracy the variation should be observed over about 1 week.
  // Note: any previous calibration should cancelled prior to any new observation period.
  // Example - RTC gaining 43 seconds in 1 week
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibration
}

//typedef union {
//   systemStateStruct state;
//   byte bytes [sizeof(systemStateStruct)];
//}systemStateUnion;

bool grabStateFromSD(systemStateStruct* input) {
  if (SD.exists("logFile.txt")) {
    File logFile = SD.open("logFile.txt", FILE_READ);
    logFile.read((uint8_t *)input, sizeof(systemStateStruct));
    logFile.close();
    return true;
  }
  else {
    return false;
  }
}

void saveStateToSD(systemStateStruct* input) {
  File logFile = SD.open("logFile.txt", FILE_WRITE);
  logFile.seek(0); // overwrite existing file
  logFile.write((uint8_t *)input, sizeof(systemStateStruct));
  logFile.close();
}

//bool checkForSysFault(){
//  if(SD.exists("logFile.txt")){
//    myFile = SD.open("logFile.txt", FILE_WRITE);
//
//
//    myFile.close();
//  }
//  Serial.print
//  myFile = SD.open("savefile.txt", FILE_WRITE);
//}

void shutOffDateTimers() {
  //  Alarm.disable(Alarm_Day1_ID);
  //  Alarm.disable(Alarm_Day2_ID);
  //  Alarm.disable(Alarm_Day3_ID);
  //  Alarm.disable(Alarm_Day4_ID);
  //  Alarm.disable(Alarm_Day5_ID);
  //  Alarm.disable(Alarm_Day6_ID);
  //  Alarm.disable(Alarm_Day7_ID);
  //  Alarm.disable(Alarm_Day8_ID);
  //  Alarm.disable(Alarm_Day9_ID);
}

<<<<<<< Updated upstream
/* Function to start system */
void forceSystemStart(){
=======
void forceSystemStart() {
>>>>>>> Stashed changes
  //disable existing timers
  shutOffDateTimers();

  // start new timers for days 2-9
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT, day_2);
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT * 2, day_3);
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT * 3, day_4);
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT * 4, day_5);
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT * 5, day_6);
  Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT * 6, day_7);
  //  Alarm.timerOnce(SECONDS_IN_DAY*7,day_8);
  //  Alarm.timerOnce(SECONDS_IN_DAY*8,day_9);

  // start first day
  day_1();
  //day_0();

}

<<<<<<< Updated upstream
/* Function for resuming system after unscheduled power loss */

void recoverSystemStart(){

  /* (1) check for hard faults */
  //  (1.1) if system failed during an experiment, generate fault
  if(systemStateStructVar.testDayComplete == 1){
    Serial.println("System was interrupted during an experiment! No way to resume!");
    while(1);
    return;
  }
  
  //  (1.2) if system paused for longer than test day interval, generate fault
  uint32_t timeSinceLastExperiment = rtc.now().unixtime() - systemStateStructVar.epoch;
  if(timeSinceLastExperiment >= DELAY_SECONDS_PER_EXPERIMENT){
    Serial.println("System was off for longer than experiment interval! No way to resume!");
    while(1);
    return;
  }

  //  (1.3) check if tests had already ended
  if(systemStateStructVar.testDay == TOTAL_EXPERIMENTS - 1){
    Serial.println("Power failure occured after experiment finished! No reason to resume!");
    while(1);
    return;
  }

  /* (2) setup timers */
  uint32_t timeRemaining = DELAY_SECONDS_PER_EXPERIMENT - timeSinceLastExperiment;
  uint16_t inc = 0;
  for(int i = 1+systemStateStructVar.testDay; i<TOTAL_EXPERIMENTS; i++){
    Alarm.timerOnce(timeRemaining + inc*DELAY_SECONDS_PER_EXPERIMENT,experimentArray[i]);
    inc++;
  }

  Serial.println("System has recovered gracefully!");
}


bool checkForStartSerialCommand(){
  
=======
bool checkForStartSerialCommand() {

>>>>>>> Stashed changes
  bool startCondition = false;
  while (Serial.available() > 0) {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';

      if (strcmp(message, "START") == 0) {
        startCondition = true;

        //Print the message (or do other things)
        Serial.println("Start Message Received");
      }

      if (strcmp(message, "S") == 0) {
        startCondition = true;
        day_0();

        //Print the message (or do other things)
        Serial.println("Start Message Received");
      }
      //Reset for the next message
      message_pos = 0;
    }
  }

  return startCondition;
}

<<<<<<< Updated upstream

void day_0(){ 
  Serial.println("EXECUTING TEST");
  systemState = 1;

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 0;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);
  
  moveLiquid (experimentTwo, e_buffer, chamberA, 4000);

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);
}

void day_1(){ 
=======
void day_0() {
  Serial.println("EXECUTING TEST");
  moveLiquid (experimentTwo, e_buffer, chamberA, 4000);

}

void day_1() {
  systemState = 1;
>>>>>>> Stashed changes

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 1;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("EXECUTING DAY_1 EXP");
  experimenta_log = experimenta_log + now() + "Day1,";

  //ExperimentOne - start enzyme
  Serial.println("ExperimentOne start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne start,";

  switchCollection(experimentOne, waste);
  //[Manual]Revival Chamber to Chamber B - 20mL
  //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, enzyme, chamberA, 75);
  //buffer to chamber A - 2.925 ml (flexible)
  moveLiquid (experimentOne, e_buffer, chamberA, 2925);

  Serial.println("ExperimentOne finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne finished,";

  //ExperimentTwo
  Serial.println("ExperimentTwo start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo start,";

  //microbe

  //Open waste valve
  switchCollection(experimentTwo, waste);
  //[Manual]Revival Chamber to Chamber B - 20mL

  //Enzyme

  //enzyme to chamber A - 0.075 ml (=3 x 25 µL)
  moveLiquid (experimentTwo, enzyme, chamberA, 75);
  //buffer to chamber A - 2.925 ml (flexible)
  moveLiquid (experimentTwo, e_buffer, chamberA, 2925);

<<<<<<< Updated upstream
Serial.println("ExperimentTwo finished moving liquid");
experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);
=======
  Serial.println("ExperimentTwo finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";
>>>>>>> Stashed changes
}

void day_2() {
  Serial.println("EXECUTING DAY_2 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 2;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day2,";

  //ExperimentOne - nothing

  //ExperimentTwo
  Serial.println("ExperimentTwo start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo start,";

  //microbe
  //open perversative valve MRNA1 (a CTRL)
  switchCollection(experimentTwo, preservativeOne);

  //move medium to chamber B - 4ml
  //which moves chamber B to preservative MRNA1(RNA) - 4ml
  moveLiquid (experimentTwo, media, chamberB, 4000);


  Serial.println("ExperimentTwo finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

}

void day_3() {
  Serial.println("EXECUTING DAY_3 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 3;
  systemStateStructVar.testDayComplete = 0;
  systemStateStructVar.testInterval = 1;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day3,";

  //ExperimentOne - mix bacteria with enzyme and replenish enzyme
  Serial.println("ExperimentOne start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne start,";

  //Open valve to preservative MRNA1 - CTRL (shouldn’t have BKA)
  switchCollection(experimentOne, preservativeOne);
  //move chamber A to Chamber B - 3ml
  moveLiquid (experimentOne, chamberA, chamberB, 3000);
  //move medium to chamber B - 3ml
  moveLiquid (experimentOne, media, chamberB, 3000);
  //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, enzyme, chamberA, 75);
  //buffer to chamber A - 2.925 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2925);

  Serial.println("ExperimentOne finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne finished,";

  //ExperimentTwo
  Serial.println("ExperimentTwo start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo start,";

  //Enzyme
  //move buffer to chamber A -  2.950 ml
  moveLiquid (experimentTwo, e_buffer, waste, 2950);
  //move enzyme to chamber A - 0.075ml
  moveLiquid (experimentTwo, enzyme, chamberA, 75);

  Serial.println("ExperimentTwo finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";

<<<<<<< Updated upstream
  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  }
=======
}
>>>>>>> Stashed changes

void day_4() {
  Serial.println("EXECUTING DAY_4 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 4;
  systemStateStructVar.testDayComplete = 0;
  systemStateStructVar.testInterval = 1;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day4,";

  //ExperimentOne - nothing

  //ExperimentTwo
  Serial.println("ExperimentTwo start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo start,";

  //Microbe
  //move medium to chamber B - 4ml
  //which moves chamber B to preservative MRNA1(RNA) - 4ml
  moveLiquid (experimentTwo, media, chamberB, 4000);

  //[proteomics]:open perversative valve PROT1
  switchCollection(experimentTwo, preservativeTwo);

  //move medium to chamber B - 4ml
  //which moves chamber B to preservative PROT1(RNA) - 4ml
  moveLiquid (experimentTwo, media, chamberB, 4000);

  Serial.println("ExperimentTwo finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);


}


void day_5() {
  Serial.println("EXECUTING DAY_5 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 5;
  systemStateStructVar.testDayComplete = 0;
  systemStateStructVar.testInterval = 1;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day5,";

  //ExperimentOne - mix bacteria with enzyme and replenish enzyme
  Serial.println("ExperimentOne start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne start,";

  //Open valve to preservative MRNA1 - with BKA
  switchCollection(experimentOne, preservativeOne);
  //move chamber A to Chamber B - 3ml
  moveLiquid (experimentOne, chamberA, chamberB, 3000);
  //move medium to chamber B - 3ml
  moveLiquid (experimentOne, media, chamberB, 3000);
  //Open valve to preservative PROT1 - with BKA
  switchCollection(experimentOne, preservativeTwo);
  //move medium to chamber B - 4ml
  moveLiquid (experimentOne, media, chamberB, 4000);
  //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, enzyme, chamberA, 75);
  //buffer to chamber A - 2.925 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2925);

<<<<<<< Updated upstream
  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

=======
  Serial.println("ExperimentOne finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne finished,";
>>>>>>> Stashed changes


}

void day_6() {
  Serial.println("EXECUTING DAY_6 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 6;
  systemStateStructVar.testDayComplete = 0;
  systemStateStructVar.testInterval = 1;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day6,";

  //ExperimentOne - nothing

  //ExperimentTwo
  Serial.println("ExperimentTwo start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo start,";

  //open perversative valve MRNA1
  switchCollection(experimentTwo, preservativeOne);

  //move medium to chamber B - 4ml
  //which moves chamber B to preservative MRNA3(RNA) - 4ml
  moveLiquid (experimentTwo, media, chamberB, 4000);

  Serial.println("ExperimentTwo finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentTwo finished,";

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

}

void day_7() {
  Serial.println("EXECUTING DAY_7 EXP");

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 7;
  systemStateStructVar.testDayComplete = 0;
  systemStateStructVar.testInterval = 1;
  saveStateToSD(&systemStateStructVar);

  experimenta_log = experimenta_log + now() + "Day7,";

  //ExperimentOne - collection
  Serial.println("ExperimentOne start moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne start,";

  //Open valve to preservative MRNA1 - with BKA
  switchCollection(experimentOne, preservativeOne);
  //move chamber A to Chamber B - 3ml
  moveLiquid (experimentOne, chamberA, chamberB, 3000);
  //move medium to chamber B - 3ml
  moveLiquid (experimentOne, media, chamberB, 3000);

  Serial.println("ExperimentOne finished moving liquid");
  experimenta_log = experimenta_log + now() + "ExperimentOne finished,";

  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);


}



void moveLiquid(int experiment, int origin, int target, float liquid_volume)
{
  //experiment ONE

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
    experimenta_log = experimenta_log + now() + "_move to chamberA," + move_pulse * 25 + "uL";
    for (int i = 0; i <= move_pulse; i++) {
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
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("move to chamberB");
    experimenta_log = experimenta_log + now() + "_move to chamberB," + move_pulse * 25 + "uL";
    for (int i = 0; i <= move_pulse; i++) {
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

  //experimentTWO

  if (experiment == 2 && origin == 0)
  {
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Experiment 2 valveA
    Serial.println("move from e_buffer");
    experimenta_log = experimenta_log + now() + "_move from e_buffer,";
  }

  if (experiment == 2 && origin == 1)
  {
    switch_conn_pin(BASEBD_J12_PIN2, HIGH); //Experiment 2 valveA
    experimenta_log = experimenta_log + now() + "_move from enzyme,";
    Serial.println("move from Enzyme");
  }

  if (experiment == 2 && target == 2)
  {
    switch_conn_pin(BASEBD_J11_PIN2, LOW); //Experiment 2 valveB low, keep in the chamber A
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("move to chamberA");
    experimenta_log = experimenta_log + now() + "_move to chamberA," + move_pulse * 25 + "uL";
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J12_PIN4, HIGH); //Experiment 2 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J12_PIN4, LOW); //Experiment 2 pumpA
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
    }
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Reset xperiment 1 valveA so enzyme is blocked
  }

  if (experiment == 2 && target == 0)
  {
    switch_conn_pin(BASEBD_J11_PIN2, HIGH); //Experiment 2 valveB high, go to waste
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("move to waste");
    experimenta_log = experimenta_log + now() + "_move to waste," + move_pulse * 25 + "uL";
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J12_PIN4, HIGH); //Experiment 2 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J12_PIN4, LOW); //Experiment 2 pumpA
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
    }
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Reset xperiment 2 valveA so enzyme is blocked
  }

  if (experiment == 2 && origin == 3 && target == 4)
  {
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("media move to chamberB");
    experimenta_log = experimenta_log + now() + "_move to chamberB," + move_pulse * 25 + "uL";
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J11_PIN4, HIGH); //Experiment 2 pumpB
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J11_PIN4, LOW); //Experiment 2 pumpB
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
    }
  }

}

void switchCollection(int experiment, int bag)
{
  if (experiment == 1 && bag == 0)
  {
    switch_conn_pin(BASEBD_J4_PIN2, LOW); //Experiment 1 waste
    Serial.println("open waste bag");
    experimenta_log = experimenta_log + now() + "_opem waste bag,";
  }
  if (experiment == 1 && bag == 1)
  {
    switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT waste
    switch_conn_pin(BASEBD_J2_PIN4, LOW); //Experiment 1 preservativeOne
    Serial.println("open preservativeOne bag");
    experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
  }
  if (experiment == 1 && bag == 2)
  {
    switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT waste
    switch_conn_pin(BASEBD_J2_PIN4, HIGH); ////Experiment 1 preservativeTwo
    Serial.println("open preservativeTwo bag");
    experimenta_log = experimenta_log + now() + "_open preservativeTwo bag,";
  }
  if (experiment == 2 && bag == 0)
  {
    switch_conn_pin(BASEBD_J3_PIN4, LOW); //Experiment 2 waste
    Serial.println("open waste bag");
    experimenta_log = experimenta_log + now() + "_opem waste bag,";
  }
  if (experiment == 2 && bag == 1)
  {
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT waste
    switch_conn_pin(BASEBD_J5_PIN2, LOW); //Experiment 2 preservativeOne
    Serial.println("open preservativeOne bag");
    experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
  }
  if (experiment == 2 && bag == 2)
  {
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT waste
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); ////Experiment 2 preservativeTwo
    Serial.println("open preservativeTwo bag");
    experimenta_log = experimenta_log + now() + "_open preservativeTwo bag,";
  }
}

void Taking_Sensor_Data() {
  Serial.println("Taking Sensor Data");
  Serial.print("Time is now: ");
  Serial.println(now());
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
  if (digits < 10)
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
    myFile.print(now());
    myFile.print(',');

    //millis
    myFile.print(millis());
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

void init_card() {

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
    myFile.println("DT, millis, Temp, Pressure, Attitude, Humidity, Temp2, Spectral_Violet, Spectral_Blue, Spectral_Green, Spectral_Yellow, Spectral_Orange, Spectral_Red, Experimental_Log");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file



}
