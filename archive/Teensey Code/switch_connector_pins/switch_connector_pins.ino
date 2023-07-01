
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

/****************************************/
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
 */
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
}

void loop() {
  digitalWrite(LED, HIGH);
 
  // MCP0 - Group A
  switch_conn_pin(BASEBD_J4_PIN2, HIGH);
  switch_conn_pin(BASEBD_J4_PIN4, HIGH);
  switch_conn_pin(BASEBD_J3_PIN2, HIGH);
  switch_conn_pin(BASEBD_J3_PIN4, HIGH);
  switch_conn_pin(BASEBD_J2_PIN2, HIGH);
  switch_conn_pin(BASEBD_J2_PIN4, HIGH);
  switch_conn_pin(BASEBD_J1_PIN2, HIGH);
  switch_conn_pin(BASEBD_J1_PIN4, HIGH);

  // MCP0 - Group B
  switch_conn_pin(BASEBD_J5_PIN2, HIGH);
  switch_conn_pin(BASEBD_J5_PIN4, HIGH);
  switch_conn_pin(BASEBD_J6_PIN2, HIGH);
  switch_conn_pin(BASEBD_J6_PIN4, HIGH);
  switch_conn_pin(BASEBD_J7_PIN2, HIGH);
  switch_conn_pin(BASEBD_J7_PIN4, HIGH);
  switch_conn_pin(BASEBD_J8_PIN2, HIGH);
  switch_conn_pin(BASEBD_J8_PIN4, HIGH);
 
  // MCP1 - Group A
  switch_conn_pin(BASEBD_J12_PIN2, HIGH);
  switch_conn_pin(BASEBD_J12_PIN4, HIGH);
  switch_conn_pin(BASEBD_J11_PIN2, HIGH);
  switch_conn_pin(BASEBD_J11_PIN4, HIGH);
  switch_conn_pin(BASEBD_J10_PIN2, HIGH);
  switch_conn_pin(BASEBD_J10_PIN4, HIGH);
  switch_conn_pin(BASEBD_J9_PIN2, HIGH);
  switch_conn_pin(BASEBD_J9_PIN4, HIGH);

/*****************************************************
 * Note - comment out below if only using base board
 */
  //MCP2 - Group B
  switch_conn_pin(SENSEBD_J2_PIN1, HIGH);
  switch_conn_pin(SENSEBD_J2_PIN2, HIGH);
  switch_conn_pin(SENSEBD_J2_PIN3, HIGH);
  switch_conn_pin(SENSEBD_J2_PIN4, HIGH);
  switch_conn_pin(SENSEBD_J3_PIN1, HIGH);
  switch_conn_pin(SENSEBD_J3_PIN2, HIGH);
/*************************************************/
  delay(250);

  digitalWrite(LED, LOW);
  
  // MCP0 - Group A
  switch_conn_pin(BASEBD_J4_PIN2, LOW);
  switch_conn_pin(BASEBD_J4_PIN4, LOW);
  switch_conn_pin(BASEBD_J3_PIN2, LOW);
  switch_conn_pin(BASEBD_J3_PIN4, LOW);
  switch_conn_pin(BASEBD_J2_PIN2, LOW);
  switch_conn_pin(BASEBD_J2_PIN4, LOW);
  switch_conn_pin(BASEBD_J1_PIN2, LOW);
  switch_conn_pin(BASEBD_J1_PIN4, LOW);

  // MCP0 - Group B
  switch_conn_pin(BASEBD_J5_PIN2, LOW);
  switch_conn_pin(BASEBD_J5_PIN4, LOW);
  switch_conn_pin(BASEBD_J6_PIN2, LOW);
  switch_conn_pin(BASEBD_J6_PIN4, LOW);
  switch_conn_pin(BASEBD_J7_PIN2, LOW);
  switch_conn_pin(BASEBD_J7_PIN4, LOW);
  switch_conn_pin(BASEBD_J8_PIN2, LOW);
  switch_conn_pin(BASEBD_J8_PIN4, LOW);
 
  // MCP1 - Group A
  switch_conn_pin(BASEBD_J12_PIN2, LOW);
  switch_conn_pin(BASEBD_J12_PIN4, LOW);
  switch_conn_pin(BASEBD_J11_PIN2, LOW);
  switch_conn_pin(BASEBD_J11_PIN4, LOW);
  switch_conn_pin(BASEBD_J10_PIN2, LOW);
  switch_conn_pin(BASEBD_J10_PIN4, LOW);
  switch_conn_pin(BASEBD_J9_PIN2, LOW);
  switch_conn_pin(BASEBD_J9_PIN4, LOW);

/*****************************************************
 * Note - comment out below if only using base board
 */
  //MCP2 - Group B
  switch_conn_pin(SENSEBD_J2_PIN1, LOW);
  switch_conn_pin(SENSEBD_J2_PIN2, LOW);
  switch_conn_pin(SENSEBD_J2_PIN3, LOW);
  switch_conn_pin(SENSEBD_J2_PIN4, LOW);
  switch_conn_pin(SENSEBD_J3_PIN1, LOW);
  switch_conn_pin(SENSEBD_J3_PIN2, LOW);
/*****************************************************/
  
  delay(250);
}
