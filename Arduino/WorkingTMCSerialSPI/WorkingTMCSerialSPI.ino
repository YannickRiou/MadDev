/**
   Author Teemu Mäntykallio
   Initializes the library and turns the motor in alternating directions.
*/

#define EN_PIN    4  // Nano v3:  16 Mega:  38  //enable (CFG6)
#define DIR_PIN   13//6  //      19      55  //direction
#define STEP_PIN  15//7  //     18      54  //step
#define CS_PIN    0 //      17      64  //chip select

bool dir = true;
int flag = 0;
#include <TMC2130Stepper.h>
TMC2130Stepper driver = TMC2130Stepper(EN_PIN, DIR_PIN, STEP_PIN, CS_PIN, 16, 17, 5);




void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Start...");
 
  driver.begin(); 			// Initiate pins and registeries
  driver.rms_current(600); 	// Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  //driver.stealthChop(1); 	// Enable extremely quiet stepping
  driver.microsteps(256);
  //driver.diag1_stall(1);
  //driver.diag1_active_high(0);
  //driver.coolstep_min_speed(0xFFFFF); // 20bit max
  //driver.THIGH(0);
  //driver.semin(5);
  //driver.semax(2);
  //driver.sedn(0b01);
  //driver.sg_stall_value(40);
  digitalWrite(EN_PIN, LOW);
}


void loop()
{
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(10);

}
