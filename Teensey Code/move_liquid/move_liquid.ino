
#include <Adafruit_MCP23X17.h>

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
  //while (!Serial);
  Serial.println("MCP23xxx Blink Test!");

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

int stop_move = 0;

}

void loop() {
  moveLiquid (experimentTwo, enzyme, chamberA, 500);
 /* 
  moveLiquid (experimentTwo, e_buffer, chamberA, 100);
  moveLiquid (experimentTwo, enzyme, chamberA, 200);
   moveLiquid (experimentTwo, media, chamberB, 200);
  moveLiquid (experimentTwo, chamberA, chamberB, 200);
   */
 
  //switchCollection(experimentOne, waste);
  //switchCollection(experimentOne, preservativeOne);
  //switchCollection(experimentOne, preservativeTwo);
  
  //switchCollection(experimentTwo, waste);
  //switchCollection(experimentTwo, preservativeOne);
  //switchCollection(experimentTwo, preservativeTwo);
  //delay(5000);
  ///switchCollection(experimentTwo, preservativeOne);
  //delay(5000);
  ///switchCollection(experimentTwo, preservativeTwo);

  stop_move = 1;
 

}

void moveLiquid(int experiment, int origin, int target, float liquid_volume) 
{
  if (stop_move == 0){
  //experiment 1, stage A
     if (experiment == 1 && origin == 0)
     {
      switch_conn_pin(BASEBD_J10_PIN2, LOW); //Experiment 1 valveA
      Serial.println("move from e_buffer");
      }

     if (experiment == 1 && origin == 1)
     {
      switch_conn_pin(BASEBD_J10_PIN2, HIGH); //Experiment 1 valveA
      }

     if (experiment == 1 && target == 2)
     {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time

      for (int i = 0; i <= move_pulse; i++){
      switch_conn_pin(BASEBD_J10_PIN4, HIGH); //Experiment 1 pumpA
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J10_PIN4, LOW); //Experiment 1 pumpA
      digitalWrite(LED, LOW);
      delay(200);
       Serial.println( i * 25);
      }
      switch_conn_pin(BASEBD_J10_PIN2, LOW); //Reset xperiment 1 valveA so enzyme is blocked
     }

     if (experiment == 1 && origin == 3) // media going out
     {
      switch_conn_pin(BASEBD_J9_PIN2, LOW); //Experiment 1 valveB
      }
     if (experiment == 1 && origin == 2) // chamberA going out
     {
      switch_conn_pin(BASEBD_J9_PIN2, HIGH); //Experiment 1 valveB
      }
     if (experiment == 1 && target == 4) 
     {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 35uL at a time

      for (int i = 0; i <= move_pulse; i++){
      switch_conn_pin(BASEBD_J9_PIN4, HIGH); //Experiment 1 pumpB
      digitalWrite(LED, HIGH);
      delay(200);
      switch_conn_pin(BASEBD_J9_PIN4, LOW); //Experiment 1 pumpB
      digitalWrite(LED, LOW);
      delay(200);
      Serial.println( i * 25);
      } 
      switch_conn_pin(BASEBD_J9_PIN2, LOW); //Reset experiment 1 valveB so chamberA is blocked  
     }
 //experiment 2, stage A
  if (experiment == 2 && origin == 0)
     {
      switch_conn_pin(BASEBD_J12_PIN2, LOW); //Experiment 2 valveA
      Serial.println("move from e_buffer");
      }

     if (experiment == 2 && origin == 1)
     {
      switch_conn_pin(BASEBD_J12_PIN2, HIGH); //Experiment 2 valveA
      }

     if (experiment == 2 && target == 2)
     {
      switch_conn_pin(BASEBD_J11_PIN2, LOW); //Experiment 2 valveB low, keep in the chamber A
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 25uL at a time
      for (int i = 0; i <= move_pulse; i++){
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
      for (int i = 0; i <= move_pulse; i++){
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
     
     if (experiment == 2 && origin == 3 && target == 4) 
     {
      int move_pulse = liquid_volume / 25; //1ml = 1000uL and the pump moves 35uL at a time
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
}

void switchCollection(int experiment, int bag)
{
  if (stop_move == 0){
   if (experiment == 1 && bag == 0)
     {
      switch_conn_pin(BASEBD_J4_PIN2, LOW); //Experiment 1 waste
      Serial.println("open waste bag");
   //   experimenta_log = experimenta_log + now() + "_opem waste bag,";
      }
   if (experiment == 1 && bag == 1)
     {
      switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT waste
      switch_conn_pin(BASEBD_J2_PIN4, LOW); //Experiment 1 preservativeOne
      Serial.println("open preservativeOne bag");
  //    experimenta_log = experimenta_log + now() + "_open preservativeOne bag,";
      }
   if (experiment == 1 && bag == 2)
     {
      switch_conn_pin(BASEBD_J4_PIN2, HIGH); //NOT waste
      switch_conn_pin(BASEBD_J2_PIN4, HIGH); ////Experiment 1 preservativeTwo
      Serial.println("open preservativeTwo bag");
    //  experimenta_log = experimenta_log + now() + "_open preservativeTwo bag,";
      }
   if (experiment == 2 && bag == 0)
     {
   //   switch_conn_pin(BASEBD_J3_PIN2, LOW); //Experiment 2 waste
      switch_conn_pin(BASEBD_J3_PIN4, LOW); //Experiment 2 waste
      Serial.println("open waste bag");
      }
     
   if (experiment == 2 && bag == 1)
     {
      switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT waste
      switch_conn_pin(BASEBD_J5_PIN2, LOW); //Experiment 2 preservativeOne
      Serial.println("open preservativeOne bag");
  //    experimenta_log = experimenta_log + now() + "_open preservativeFour bag,";
      }
   if (experiment == 2 && bag == 2)
     {
      switch_conn_pin(BASEBD_J3_PIN4, HIGH); //NOT waste
      switch_conn_pin(BASEBD_J5_PIN2, HIGH); ////Experiment 2 preservativeTwo
      Serial.println("open preservativeTwo bag");
      //experimenta_log = experimenta_log + now() + "_open preservativeFive bag,";
      }  
  }   
  }
