/***************************************************************************
  Base Board Connection:
  E1VA + E1PA - J10
  E1VB + E1PB - J09
  E2VA + E2PA - J12
  E2VB + E2PB - J11
  the valve board connector J1-6 are the same on the base board
s
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

/* Experiment Prototypes and supporting defines */
#define TOTAL_EXPERIMENTS 26
void day_1();
void day_2();
void day_3();
void day_4();
void day_5();
void day_6();
void day_7();
void day_8();
void day_9();
void day_10();
void day_11();
void day_12();
void day_13();
void day_14();
void day_15();
void day_16();
void day_17();
void day_18();
void day_19();
void day_20();
void day_21();
void day_22();
void day_23();
void day_24();
void day_25();
void day_26();

void (*experimentArray[TOTAL_EXPERIMENTS])() = {day_1, day_2, day_3, day_4, day_5, day_6, day_7, day_8, day_9, day_10, day_11, day_12, day_13, day_14, day_15, day_16, day_17, day_18, day_19, day_20, day_21, day_22, day_23, day_24, day_25, day_26};
//Teensey 4.1 - SCL 19 - yellow /SDA 18 - blue

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

RTC_PCF8523 rtc;


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

uint16_t experimentIndex = 0;

unsigned long delayTime;
#define SEALEVELPRESSURE_HPA (1013.25)
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
File expLogFile;
const int chipSelect = BUILTIN_SDCARD;
const int ledPin = 5;

systemStateStruct systemStateStructVar = {0, 0, 0, 0};

//IO ports

#define MCP_ADDR0 0x20  // IC5 on base board
#define MCP_ADDR1 0x21  // IC4 on base board
#define MCP_ADDR2 0x22  // IC1 on sensor board

enum motion { forward, reverse, halt };
#define PUMP1_A 8
#define PUMP1_B 9
#define PUMP2_A 10
#define PUMP2_B 11
#define PUMP3_A 12
#define PUMP3_B 13

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
unsigned int whichsensor = 0;

#define SECONDS_IN_DAY 86400
//#define DELAY_SECONDS_PER_EXPERIMENT SECONDS_IN_DAY
#define DELAY_SECONDS_PER_EXPERIMENT 120 // 1 mins for the functional testing
#define SERIAL_TIMEOUT 10000 //milliseconds

const unsigned int MAX_MESSAGE_LENGTH = 12;
bool systemState = 0;

//AlarmID_t Alarm_Day1_ID;
//AlarmID_t Alarm_Day2_ID;
//AlarmID_t Alarm_Day3_ID;
//AlarmID_t Alarm_Day4_ID;
//AlarmID_t Alarm_Day5_ID;
//AlarmID_t Alarm_Day6_ID;
//AlarmID_t Alarm_Day7_ID;
//AlarmID_t Alarm_Day8_ID;
//AlarmID_t Alarm_Day9_ID;

// Function to select a sensor
void tcaselect(uint8_t sensor_addr) {
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << sensor_addr);
  Wire.endTransmission();
}

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
  initExpLogFile();
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

  // IC1 on sensor board
  mcp2.pinMode(GPB0, OUTPUT);
  mcp2.pinMode(GPB1, OUTPUT);
  mcp2.pinMode(GPB2, OUTPUT);
  mcp2.pinMode(GPB3, OUTPUT);
  mcp2.pinMode(GPB4, OUTPUT);
  mcp2.pinMode(GPB5, OUTPUT);

  Serial.println("Looping...");
*/


// Set up the sensors
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
    }

    if (!mcp2.begin_I2C(MCP_ADDR2)) {
      Serial.println("Error initializing chip 2.");
    }

    Serial.println("Looping...");

  // EVERYDAY TASK
  Alarm.timerRepeat(60, Taking_Sensor_Data);


}


//  for(int i = 1+systemStateStructVar.testDay; i<TOTAL_EXPERIMENTS; i++){
//    Alarm.timerOnce(timeRemaining + inc*DELAY_SECONDS_PER_EXPERIMENT,(*experimentArray[i]));
//    inc++;
//  }

void loop() {

  //  digitalClockDisplay();
  Alarm.delay(1000);

  // if system has not already started, check for start condition via serial
  if (!systemState && checkForStartSerialCommand()) {

    // system has been commanded to start
    day_1();

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

/* Function for resuming system after unscheduled power loss */

void recoverSystemStart(){

  /* (1) check for hard faults */
  //  (1.1) if system failed during an experiment, generate fault
  if(systemStateStructVar.testDayComplete != 1){
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
//  uint16_t inc = 0;
  experimentIndex = systemStateStructVar.testDay;
  Alarm.timerOnce(timeRemaining,(*experimentArray[experimentIndex]));
//  for(int i = 1+systemStateStructVar.testDay; i<TOTAL_EXPERIMENTS; i++){
//    Alarm.timerOnce(timeRemaining + inc*DELAY_SECONDS_PER_EXPERIMENT,(*experimentArray[i]));
//    inc++;
//  }

  Serial.println("System has recovered gracefully!");
}


bool checkForStartSerialCommand() {
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


      if (strcmp(message, "TEST") == 0) {
        //Print the message (or do other things)
        Serial.println("Executing Test Script");

        day_functionTest();
      }

      //Reset for the next message
      message_pos = 0;
    }
  }

  return startCondition;
}

void day_functionTest(){

}


void day_1() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();
  
//==================================================
//==================================================
//==================================================
//==================================================
//==================================================
// =============== UPDATE STATUS ===================
  systemState = 1;

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 1;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 1");
  saveExperimentalLog("Executing Day 1");


 // =============== SAFETY MECHANISM ================
  // DUMMY TASK

  //EXP1
  moveLiquid (experimentOne, e_buffer, chamberA, 2000);  //buffer to chamber A - 2 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, e_buffer, chamberA, 2000);  //buffer to chamber A - 2 ml (flexible)


 // =============== MICROBE EXP =====================

  //EXP1
  Serial.println("EXP1 revival pumping...");
  saveExperimentalLog("EXP1 revived");
  switchCollection(experimentOne, waste);//back to waste bag
  motorSensorBrdCtrl(1, forward);
  delay (5000); //~10ML
  motorSensorBrdCtrl(1, halt);

  //EXP2
  Serial.println("EXP2 revival pumping...");
  saveExperimentalLog("EXP2 revived");
  switchCollection(experimentTwo, waste);//back to waste bag
  motorSensorBrdCtrl(3, forward);
  delay (5000); //~10ML
  motorSensorBrdCtrl(3, halt);


 // =============== ENZYME EXP =====================

  //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE STATUS ===================
  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 1 Finishes");
  saveExperimentalLog("Day 1 Finishes");
}

void day_2() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 2;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 1");
  saveExperimentalLog("Executing Day 1");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 2 Finishes");
  saveExperimentalLog("Day 2 Finishes");



}

void day_3() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 3;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 3");
  saveExperimentalLog("Executing Day 3");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

   Serial.println("Day 3 Finishes");
  saveExperimentalLog("Day 3 Finishes");



}

void day_4() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 4;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 4");
  saveExperimentalLog("Executing Day 4");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

  //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 4 Finishes");
  saveExperimentalLog("Day 4 Finishes");


}

void day_5() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 5;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 5");
  saveExperimentalLog("Executing Day 5");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 5 Finishes");
  saveExperimentalLog("Day 5 Finishes");


}

void day_6() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 6;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 6");
  saveExperimentalLog("Executing Day 6");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeOne);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag

 //EXP2
  switchCollection(experimentTwo, preservativeOne);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 6 Finishes");
  saveExperimentalLog("Day 6 Finishes");


}

void day_7() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 7;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 7");
  saveExperimentalLog("Executing Day 7");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 7 Finishes");
  saveExperimentalLog("Day 7 Finishes");


}

void day_8() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 8;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 8");
  saveExperimentalLog("Executing Day 8");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 8 Finishes");
  saveExperimentalLog("Day 8 Finishes");


}

void day_9() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 9;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 9");
  saveExperimentalLog("Executing Day 9");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 9 Finishes");
  saveExperimentalLog("Day 9 Finishes");


}

void day_10() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 10;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 10");
  saveExperimentalLog("Executing Day 10");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeOne);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  Serial.println("Exp1_SDS collection");
  saveExperimentalLog("Exp1_SDS collection");

  switchCollection(experimentOne, preservativeFour);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 10000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag



 //EXP2
  switchCollection(experimentTwo, preservativeOne);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  Serial.println("Exp2_SDS collection");
  saveExperimentalLog("Exp2_SDS collection");

  switchCollection(experimentTwo, preservativeFour);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 10000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag


 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 10 Finishes");
  saveExperimentalLog("Day 10 Finishes");


}

void day_11() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 11;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 11");
  saveExperimentalLog("Executing Day 11");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 11 Finishes");
  saveExperimentalLog("Day 11 Finishes");


}

void day_12() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 12;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 12");
  saveExperimentalLog("Executing Day 12");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 12 Finishes");
  saveExperimentalLog("Day 12 Finishes");


}

void day_13() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 13;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 13");
  saveExperimentalLog("Executing Day 13");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.525 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 13 Finishes");
  saveExperimentalLog("Day 13 Finishes");


}

void day_14() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 14;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 14");
  saveExperimentalLog("Executing Day 14");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeTwo);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag

 //EXP2
  switchCollection(experimentTwo, preservativeTwo);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 14 Finishes");
  saveExperimentalLog("Day 14 Finishes");


}

void day_15() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 15;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 15");
  saveExperimentalLog("Executing Day 15");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 15 Finishes");
  saveExperimentalLog("Day 15 Finishes");


}

void day_16() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 16;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 16");
  saveExperimentalLog("Executing Day 16");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 16 Finishes");
  saveExperimentalLog("Day 16 Finishes");


}

void day_17() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 17;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 17");
  saveExperimentalLog("Executing Day 17");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 17 Finishes");
  saveExperimentalLog("Day 17 Finishes");


}

void day_18() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 18;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 18");
  saveExperimentalLog("Executing Day 18");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeTwo);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag

 //EXP2
  switchCollection(experimentTwo, preservativeTwo);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 18 Finishes");
  saveExperimentalLog("Day 18 Finishes");


}

void day_19() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 19;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 19");
  saveExperimentalLog("Executing Day 19");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 19 Finishes");
  saveExperimentalLog("Day 19 Finishes");


}

void day_20() {
  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 20;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 20");
  saveExperimentalLog("Executing Day 20");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 20 Finishes");
  saveExperimentalLog("Day 20 Finishes");


}

void day_21() {

  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 21;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 21");
  saveExperimentalLog("Executing Day 21");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 21 Finishes");
  saveExperimentalLog("Day 21 Finishes");


}

void day_22() {

  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 22;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 22");
  saveExperimentalLog("Executing Day 22");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeThree);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag

 //EXP2
  switchCollection(experimentTwo, preservativeThree);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag


 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 22 Finishes");
  saveExperimentalLog("Day 22 Finishes");


}

void day_23() {

  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 23;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 23");
  saveExperimentalLog("Executing Day 23");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 23 Finishes");
  saveExperimentalLog("Day 23 Finishes");


}

void day_24() {

  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 24;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 24");
  saveExperimentalLog("Executing Day 24");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 24 Finishes");
  saveExperimentalLog("Day 24 Finishes");


}

void day_25() {

  // =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============

  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 25;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 25");
  saveExperimentalLog("Executing Day 25");


 // =============== MICROBE EXP =====================

 // DO NOTHING

 // =============== ENZYME EXP =====================

 //EXP1
  moveLiquid (experimentOne, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentOne, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)
  //EXP2
  moveLiquid (experimentTwo, enzyme, chamberA, 75); //enzyme to chamber A - 0.075 ml
  moveLiquid (experimentTwo, e_buffer, chamberA, 2525);  //buffer to chamber A - 2.925 ml (flexible)


 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 25 Finishes");
  saveExperimentalLog("Day 25 Finishes");


}

void day_26() {

// =============== SET == NEXT == ALARM ============ 
  setNextAlarm();

// =============== UPDATE === STATUS ===============
  systemStateStructVar.experimentStarted = 1;
  systemStateStructVar.epoch = rtc.now().unixtime();
  systemStateStructVar.testDay = 26;
  systemStateStructVar.testInterval = 1;
  systemStateStructVar.testDayComplete = 0;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Executing Day 26");
  saveExperimentalLog("Executing Day 26");


 // =============== MICROBE EXP =====================

 //EXP1
  switchCollection(experimentOne, preservativeThree);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentOne, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentOne, waste);//back to waste bag

 //EXP2
  switchCollection(experimentTwo, preservativeThree);//Open valve to preservative MRNA1 - with BKA
  moveLiquid (experimentTwo, media, chamberB, 5000);//move medium to chamber B - 6ml
  switchCollection(experimentTwo, waste);//back to waste bag


 // =============== ENZYME EXP =====================

 // DO NOTHING

 // =============== UPDATE === STATUS ===============


  systemStateStructVar.testDayComplete = 1;
  saveStateToSD(&systemStateStructVar);

  Serial.println("Day 26 Finishes");
  saveExperimentalLog("Day 26 Finishes");


}





void moveLiquid(int experiment, int origin, int target, float liquid_volume)
{
   //experiment ONE_Enyzme Part

  if (experiment == 1 && origin == 0)
  {
    switch_conn_pin(BASEBD_J10_PIN2, LOW); //Experiment 1 valveA
    Serial.println("_E1_exp1 move from e_buffer");
    saveExperimentalLog("_E1_move from e_buffer");
  }

  if (experiment == 1 && origin == 1)
  {
    switch_conn_pin(BASEBD_J10_PIN2, HIGH); //Experiment 1 valveA
    Serial.println("_E1 move from enzyme");
    saveExperimentalLog("_E1_move from enzyme");
  }

  if (experiment == 1 && target == 2)
  {
    switch_conn_pin(BASEBD_J9_PIN2, HIGH); //Experiment 1 valveB high, waste line open
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("move to chamberA");
    saveExperimentalLog("exp1_move to chamberA" + String(move_pulse * 25) + "uL");
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
  /* Uncomment and edit line above switch_conn_pin(BASEBD_J9_PIN2, HIGH); to LOW
 if we want to control chamberA and waste flow
  if (experiment == 1 && target == 0)
  {
    switch_conn_pin(BASEBD_J9_PIN2, HIGH); //Experiment 2 valveB high, go to waste
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("move to waste");
    saveExperimentalLog("_move to waste " + move_pulse * 25 + "uL");
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J10_PIN4, HIGH); //Experiment 2 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J10_PIN4, LOW); //Experiment 2 pumpA
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
    }
    switch_conn_pin(BASEBD_J10_PIN2, LOW); //Reset experiment 2 valveA so enzyme is blocked
  }
*/

//experiment One_BioCell Part

  if (experiment == 1 && origin == 3 && target == 4)
  {
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("_E1 move to chamberB");
    saveExperimentalLog("_E1_move to chamberB " + String(move_pulse * 25) + "uL");
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J9_PIN4, HIGH); //Experiment 1 pumpB
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J9_PIN4, LOW); //Experiment 1 pumpB
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25); //print out the moving volume
    }
  //  switch_conn_pin(BASEBD_J9_PIN2, LOW); //Reset experiment 1 valveB so chamberA is blocked
  }

  //experimentTWO, Enzyme

  if (experiment == 2 && origin == 0)
  {
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Experiment 2 valveA
    Serial.println("_E2 move from e_buffer");
    saveExperimentalLog("_E2_move from e_buffer");
  }

  if (experiment == 2 && origin == 1)
  {
    switch_conn_pin(BASEBD_J12_PIN2, HIGH); //Experiment 2 valveA
    saveExperimentalLog("_E2_move from enzyme");
    Serial.println("_E2 move from Enzyme");
  }

  if (experiment == 2 && target == 2)
  {
    switch_conn_pin(BASEBD_J11_PIN2, HIGH); //Experiment 2 valveB high, waste line open
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("_E2 move to chamberA");
    saveExperimentalLog("_E2_move to chamberA " + String(move_pulse * 25) + "uL");
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

  /* Uncomment and edit line above switch_conn_pin(BASEBD_J11_PIN2, HIGH); to LOW
  if we want to control chamberA and waste flow
    if (experiment == 2 && target == 0)
    {
      switch_conn_pin(BASEBD_J11_PIN2, HIGH); //Experiment 2 valveB high, go to waste
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
      Serial.println("move to waste");
      saveExperimentalLog("_move to waste " + move_pulse * 25 + "uL");
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
  */

  //experimentTWO, Microbe

  if (experiment == 2 && origin == 3 && target == 4)
   {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 35uL at a time
      Serial.println("_E2 media move to chamberB");
      saveExperimentalLog("_E2_media_move to chamberB " + String(move_pulse * 25) + "uL");
      for (int i = 0; i <= move_pulse; i++){
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
    switch_conn_pin(BASEBD_J5_PIN4, LOW); //Experiment 1 waste V6
    Serial.println("exp1 open waste bag");
    saveExperimentalLog("exp1_opem waste bag");
  }
  if (experiment == 1 && bag == 1)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, LOW); //Experiment 1 preservativeOne V5
    Serial.println("exp1 open preservativeOne bag");
    saveExperimentalLog("exp1_open preservativeOne bag");
  }
  if (experiment == 1 && bag == 2)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, LOW); ////Experiment 1 preservativeTwo V4
    Serial.println("exp1 open preservativeTwo bag");
    saveExperimentalLog("exp1_open preservativeTwo bag");
  }
  if (experiment == 1 && bag == 3)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT V4
    switch_conn_pin(BASEBD_J3_PIN2, LOW); //Experiment 1 preservativeThree V3
    Serial.println("open preservativeThree bag");
    saveExperimentalLog("exp1_open preservativeThree bag");
  }
    if (experiment == 1 && bag == 4)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT V4
    switch_conn_pin(BASEBD_J3_PIN2, HIGH); //NOT V3
    switch_conn_pin(BASEBD_J1_PIN4, LOW); //Experiment 1 preservativeFour V2
    Serial.println("open preservativeFour bag");
    }

  //experiment two collection

  if (experiment == 2 && bag == 0)
  {
    switch_conn_pin(BASEBD_J2_PIN2, LOW); //Experiment 2 waste V7
    Serial.println("exp2 open waste bag");
    saveExperimentalLog("exp2_opem waste bag");
  }
  if (experiment == 2 && bag == 1)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, LOW); //Experiment 2 preservativeOne V8
    Serial.println("exp2 open preservativeOne bag");
    saveExperimentalLog("exp2_open preservativeOne bag");
  }
  if (experiment == 2 && bag == 2)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
    switch_conn_pin(BASEBD_J4_PIN2, LOW); ////Experiment 2 preservativeTwo V9
    Serial.println("exp2 open preservativeTwo bag");
    saveExperimentalLog("exp2_open preservativeTwo bag");
  }
  if (experiment == 2 && bag == 3)
  {
  switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
  switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
  switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT V9
  switch_conn_pin(BASEBD_J4_PIN4, LOW); //Experiment 2 preservativeThree V10
  Serial.println("open preservativeThree bag");
  saveExperimentalLog("exp2_open preservativeThree bag");
  }
if (experiment == 2 && bag == 4)
{
  switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
  switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
  switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT V9
  switch_conn_pin(BASEBD_J4_PIN4, HIGH); //NOT V10
  switch_conn_pin(BASEBD_J6_PIN2, LOW); //Experiment 2 preservativeFour V11
  Serial.println("open preservativeFour bag");
  saveExperimentalLog("exp2_open preservativeFour bag");
}
}

void Taking_Sensor_Data() {
  Serial.println("Taking Sensor Data");
  Serial.print("Time is now: ");
  Serial.println(now());
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
//    myFile.print(',');
//
//    //EXP Log
//    myFile.print(experimenta_log);

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

void saveExperimentalLog(String logmessage){
  expLogFile = SD.open("explog.csv", FILE_WRITE);
  if(expLogFile){
     expLogFile.print(now());
     expLogFile.print(',');
     expLogFile.print(logmessage);
     expLogFile.println();
     expLogFile.close();
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
    myFile.println("DT, millis, Sensorline, Temp, Pressure, Attitude, Humidity, Temp2, Spectral_Violet, Spectral_Blue, Spectral_Green, Spectral_Yellow, Spectral_Orange, Spectral_Red");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file
}

void initExpLogFile(){
  expLogFile = SD.open("explog.csv", FILE_WRITE);
  if (expLogFile) {
    expLogFile.print("timestamp");
     expLogFile.print(',');
     expLogFile.print("log_message");
     expLogFile.println();
     expLogFile.close();
  }
}

/* Function to control motors on sensor board */
/* Input: (1) motor_num is either 1 or 3
 *        (2) dir is either forward, reverse, or halt
 */
void motorSensorBrdCtrl(uint8_t motor_num, motion dir){
  if(motor_num == 1){
    if(dir == forward){
      mcp2.digitalWrite(PUMP1_A, HIGH);
      mcp2.digitalWrite(PUMP1_B, LOW);
    }else if(dir == reverse){
      mcp2.digitalWrite(PUMP1_A, LOW);
      mcp2.digitalWrite(PUMP1_B, HIGH);
    }else if(dir == halt){
      mcp2.digitalWrite(PUMP1_A, LOW);
      mcp2.digitalWrite(PUMP1_B, LOW);
    }
  }else if(motor_num == 3){
    if(dir == forward){
      mcp2.digitalWrite(PUMP3_A, HIGH);
      mcp2.digitalWrite(PUMP3_B, LOW);
    }else if(dir == reverse){
      mcp2.digitalWrite(PUMP3_A, LOW);
      mcp2.digitalWrite(PUMP3_B, HIGH);
    }else if(dir == halt){
      mcp2.digitalWrite(PUMP3_A, LOW);
      mcp2.digitalWrite(PUMP3_B, LOW);
    }
  }
  delay (1000);
}

void setNextAlarm(){
  experimentIndex++;
  if(experimentIndex < TOTAL_EXPERIMENTS){
    Alarm.timerOnce(DELAY_SECONDS_PER_EXPERIMENT, (*experimentArray[experimentIndex]));
  }
}
