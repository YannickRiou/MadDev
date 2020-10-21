
#include <TMC2208Stepper.h>                       // Include library

TMC2208Stepper driver = TMC2208Stepper(&Serial2);  

#define EN_PIN    5  
#define DIR_PIN 4
#define STEP_PIN  18

// Main variables for stepper operations
bool dir = true;

// Store the actual position of the camera on the dolly in steps
uint32_t positionInStep = 0;

// Define a new stepper object with pinout defined in stepper.h

void stepperInit()
{
   
  pinMode(EN_PIN,OUTPUT);
  pinMode(STEP_PIN,OUTPUT);
 pinMode(DIR_PIN,OUTPUT);
  Serial2.begin(115200);
  driver.push();                // Reset registers

  driver.pdn_disable(true);     // Use PDN/UART pin for communication
  driver.I_scale_analog(false); // Use internal voltage reference
  driver.rms_current(500);      // Set driver current 500mA
  driver.toff(2);               // Enable driver in software
  driver.microsteps(256);
  driver.en_spreadCycle(0);
}

void stepperEnable()
{
  digitalWrite(EN_PIN, LOW);
}

void stepperDisable()
{
  digitalWrite(EN_PIN, HIGH);
}

void makeStep(int nbOfStep, int delayBtwn)
{
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(10);
  if(delayBtwn > 0)
  {
    delay(delayBtwn);
  }

}
