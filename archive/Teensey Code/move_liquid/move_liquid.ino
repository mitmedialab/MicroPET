
#include <Adafruit_MCP23X17.h>

#define MCP_ADDR0 0x20  // IC5 on base board
#define MCP_ADDR1 0x21  // IC4 on base board
#define MCP_ADDR2 0x22  // IC1 on sensor board

//revivial pump
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

int stop_move;

void setup() {
   Serial.begin(9600);
 // while (!Serial);
 // Serial.println("MCP23xxx Blink Test!");

  pinMode(LED, OUTPUT);

  // uncomment appropriate mcp.begin
  if (!mcp0.begin_I2C(MCP_ADDR0)) {
    Serial.println("Error initializing chip 0.");
   // while (1);
  }

  if (!mcp1.begin_I2C(MCP_ADDR1)) {
    Serial.println("Error initializing chip 1.");
    //while (1);
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

 //Revial 
 
   if (!mcp2.begin_I2C(MCP_ADDR2)) {
    Serial.println("Error initializing chip 2.");
 //   while (1);
  }
/*
  // IC1 on sensor board
  mcp2.pinMode(GPB0, OUTPUT);
  mcp2.pinMode(GPB1, OUTPUT);
  mcp2.pinMode(GPB2, OUTPUT);
  mcp2.pinMode(GPB3, OUTPUT);
  mcp2.pinMode(GPB4, OUTPUT);
  mcp2.pinMode(GPB5, OUTPUT);

  Serial.println("Looping...");

*/

    Serial.println("revival pumping...");
    mcp2.pinMode(PUMP1_A, OUTPUT);
    mcp2.pinMode(PUMP1_B, OUTPUT);
    //    mcp.pinMode(PUMP2_A, OUTPUT);
    //    mcp.pinMode(PUMP2_B, OUTPUT);
    mcp2.pinMode(PUMP3_A, OUTPUT);
    mcp2.pinMode(PUMP3_B, OUTPUT);

    // halt both motors upon initialization
    
    motorSensorBrdCtrl(1, halt);
    motorSensorBrdCtrl(3, halt);
    Serial.println("revival pumping halted");
    
int stop_move = 0;



}

void loop() {

  Serial.println("revival pumping...");
  motorSensorBrdCtrl(1, forward);
  delay (5000);
  motorSensorBrdCtrl(1, halt);
  
//Experiment1 liquid system cleaning
// moveLiquid (experimentOne, e_buffer, chamberA, 50);
// moveLiquid (experimentOne, enzyme, chamberA, 5000);
// moveLiquid (experimentOne, media, chamberB, 5000);

//Experiment2 liquid system cleaning
// moveLiquid (experimentTwo, e_buffer, chamberA, 5000);
// moveLiquid (experimentTwo, enzyme, chamberA, 5000);
// moveLiquid (experimentTwo, media, chamberB, 5000);

//Experiment2 collection 
/*
switchCollection(experimentTwo, preservativeTwo);
delay (3000);
switchCollection(experimentTwo, preservativeThree);
delay (3000);
switchCollection(experimentTwo, preservativeFour);
delay (3000);

  Serial.println("revival pumping...");
  motorSensorBrdCtrl(1, forward);
  delay (5000);
  motorSensorBrdCtrl(1, halt);
  
  /*   
  motorSensorBrdCtrl(3, forward);
  delay (500); //8-9 times for a 10ML revival from the big media bag
  motorSensorBrdCtrl(3, halt);
  
  /*
   * 
   * moveLiquid (experimentOne, media, chamberB, 5000);
   switchCollection(experimentOne, preservativeOne);
  moveLiquid (experimentOne, media, chamberB, 5000);
   switchCollection(experimentOne, preservativeTwo);
  moveLiquid (experimentOne, media, chamberB, 5000);
  switchCollection(experimentOne, preservativeThree);
  moveLiquid (experimentOne, media, chamberB, 5000);
  switchCollection(experimentOne, preservativeFour);
  moveLiquid (experimentOne, media, chamberB, 5000);
  switchCollection(experimentOne, waste);
  moveLiquid (experimentOne, media, chamberB, 5000);
  
  delay(5000);
  moveLiquid (experimentOne, media, chamberB, 5000);
  delay(5000);
  switchCollection(experimentOne, preservativeTwo);
  moveLiquid (experimentOne, media, chamberB, 5000);
  delay(5000);
  switchCollection(experimentOne, preservativeThree);
  moveLiquid (experimentOne, media, chamberB, 5000);
  delay(5000);
  switchCollection(experimentOne, preservativeFour);
  moveLiquid (experimentOne, media, chamberB, 5000);
  delay(5000);

 */
 

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


void moveLiquid(int experiment, int origin, int target, float liquid_volume)
{
  //experiment ONE_Enyzme Part

  if (experiment == 1 && origin == 0)
  {
    switch_conn_pin(BASEBD_J10_PIN2, LOW); //Experiment 1 valveA, block enzyme
    Serial.println("_E1 move from e_buffer");
  //  saveExperimentalLog("_E1_move from e_buffer");
  }

  if (experiment == 1 && origin == 1)
  {
    switch_conn_pin(BASEBD_J10_PIN2, HIGH); //Experiment 1 valveA, allow enzyme
    Serial.println("_E1 move from enzyme");
  //  saveExperimentalLog("_E1_move from enzyme");
  }

  if (experiment == 1 && target == 2)
  {
    switch_conn_pin(BASEBD_J9_PIN2, HIGH); //Experiment 1 valveB high, waste line open
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("_E1 move to chamberA");
  //  saveExperimentalLog("_E1_move to chamberA " + move_pulse * 25 + "uL");
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
    Serial.println("_E1 media move to chamberB");
  //  saveExperimentalLog("_E1_media_move to chamberB " + move_pulse * 25 + "uL");
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J9_PIN4, HIGH); //Experiment 1 pumpB
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J9_PIN4, LOW); //Experiment 1 pumpB
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25); //print out the moving volume
    }
  //  switch_conn_pin(BASEBD_J9_PIN2, LOW); //Reset experiment 1 valveB to NC
  }

  //experimentTWO, Enzyme

  if (experiment == 2 && origin == 0)
  {
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Experiment 2 valveA, block enzyme
    Serial.println("_E2 move from e_buffer");
  //  saveExperimentalLog("_E2_move from e_buffer");
  }

  if (experiment == 2 && origin == 1)
  {
    switch_conn_pin(BASEBD_J12_PIN2, HIGH); //Experiment 2 valveA, allow enzyme
    Serial.println("_E2 move from Enzyme");
  //  saveExperimentalLog("_E2_move from enzyme");
  }

  if (experiment == 2 && target == 2)
  {
    switch_conn_pin(BASEBD_J11_PIN2, HIGH); //Experiment 2 valveB high, waste line open
    int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
    Serial.println("_E2 move to chamberA");
  //  saveExperimentalLog("_E2_move to chamberA " + move_pulse * 25 + "uL");
    for (int i = 0; i <= move_pulse; i++) {
      switch_conn_pin(BASEBD_J12_PIN4, HIGH); //Experiment 2 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J12_PIN4, LOW); //Experiment 2 pumpA
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
    }
    switch_conn_pin(BASEBD_J12_PIN2, LOW); //Reset experiment 1 valveA so enzyme is blocked
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
   //   saveExperimentalLog("_E2_media_move to chamberB " + move_pulse * 25 + "uL");
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
  if (stop_move == 0){

    //experiment one collection
   if (experiment == 1 && bag == 0)
  {
    switch_conn_pin(BASEBD_J5_PIN4, LOW); //Experiment 1 waste V6
    Serial.println("open waste bag");
 //   experimenta_log = experimenta_log + now() + "_opem waste bag,";
  }
  if (experiment == 1 && bag == 1)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, LOW); //Experiment 1 preservativeOne V5
    Serial.println("open preservativeOne bag");
  //  experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
  }
  if (experiment == 1 && bag == 2)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, LOW); ////Experiment 1 preservativeTwo V4
    Serial.println("open preservativeTwo bag");
 //   experimenta_log = experimenta_log + now() + "_open preservativeTwo bag,";
  }
    if (experiment == 1 && bag == 3)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT V4
    switch_conn_pin(BASEBD_J3_PIN2, LOW); //Experiment 1 preservativeThree V3
    Serial.println("open preservativeThree bag");
 //   experimenta_log = experimenta_log + now() + "_open preservativeThree bag,";
  }
    if (experiment == 1 && bag == 4)
  {
    switch_conn_pin(BASEBD_J5_PIN4, HIGH); //NOT V6
    switch_conn_pin(BASEBD_J5_PIN2, HIGH); //NOT V5
    switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT V4
    switch_conn_pin(BASEBD_J3_PIN2, HIGH); //NOT V3
    switch_conn_pin(BASEBD_J1_PIN4, LOW); //Experiment 1 preservativeFour V2
    Serial.println("open preservativeFour bag");
 //   experimenta_log = experimenta_log + now() + "_open preservativeFour bag,";
  }
  
  //experiment two collection
  
if (experiment == 2 && bag == 0)
  {
    switch_conn_pin(BASEBD_J2_PIN2, LOW); //Experiment 2 waste V7 
    Serial.println("open waste bag");
   // experimenta_log = experimenta_log + now() + "_opem waste bag,";
  }
  if (experiment == 2 && bag == 1)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, LOW); //Experiment 2 preservativeOne V8
    Serial.println("open preservativeOne bag");
   // experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
  }
  if (experiment == 2 && bag == 2)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
    switch_conn_pin(BASEBD_J4_PIN2, LOW); ////Experiment 2 preservativeTwo V9
    Serial.println("open preservativeTwo bag");
  //  experimenta_log = experimenta_log + now() + "_open preservativeTwo bag,";
  }
  if (experiment == 2 && bag == 3)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
    switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT V9
    switch_conn_pin(BASEBD_J4_PIN4, LOW); //Experiment 2 preservativeThree V10
    Serial.println("open preservativeThree bag");
  //  experimenta_log = experimenta_log + now() + "_open preservativeThree bag,";
  }
  if (experiment == 2 && bag == 4)
  {
    switch_conn_pin(BASEBD_J2_PIN2, HIGH); //NOT V7
    switch_conn_pin(BASEBD_J2_PIN4, HIGH); //NOT V8
    switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT V9
    switch_conn_pin(BASEBD_J4_PIN4, HIGH); //NOT V10
    switch_conn_pin(BASEBD_J6_PIN2, LOW); //Experiment 2 preservativeFour V11
    Serial.println("open preservativeFour bag");
  //  experimenta_log = experimenta_log + now() + "_open preservativeFour bag,";
  }
  }   
  }
