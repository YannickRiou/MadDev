// Author Teemu Mäntykallio, 2017-04-07

// Define pins
#define EN_PIN    5  // LOW: Driver enabled. HIGH: Driver disabled
#define STEP_PIN  6  // Step on rising edge
#define RX_PIN    9  // SoftwareSerial pins
#define TX_PIN    8  //

#define DIR_PIN    11  //

int i  = 0;
#include <TMC2208Stepper.h>

// Create driver that uses SoftwareSerial for communication
TMC2208Stepper driver = TMC2208Stepper(RX_PIN, TX_PIN);

void setup() {
  driver.beginSerial(115200);
  // Push at the start of setting up the driver resets the register to default
  driver.push();
  // Prepare pins
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Start...");
  
  driver.pdn_disable(true);     // Use PDN/UART pin for communication
  driver.I_scale_analog(false); // Use internal voltage reference
  driver.rms_current(800);      // Set driver current = 500mA, 0.5 multiplier for hold current and RSENSE = 0.11.
  driver.toff(2);               // Enable driver in software
  driver.en_spreadCycle(0); // Stealthchop enabled
  driver.mstep_reg_select(1);
  driver.microsteps(256);
  digitalWrite(EN_PIN, LOW);    // Enable driver in hardware

  uint32_t data = 0;
  Serial.print("DRV_STATUS = 0x");
  driver.DRV_STATUS(&data);
    Serial.println(data, HEX);

  digitalWrite(DIR_PIN,HIGH);
    for(i=0; i<1000;i++)
  {
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  delay(1);
  }
  digitalWrite(DIR_PIN,LOW);
    for(i=0; i<1000;i++)
  {
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  delay(1);
  }
}


void loop() {
  digitalWrite(DIR_PIN,HIGH);
    for(i=0; i<10000;i++)
  {
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  delay(1);
  }
   uint32_t data = 0;
  Serial.print("IOIN = 0x");
  driver.IOIN(&data);
    Serial.println(data, HEX);

  digitalWrite(DIR_PIN,LOW);
    for(i=0; i<10000;i++)
  {
  digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
  delay(1);
  }
 
  
}
