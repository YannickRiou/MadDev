// Define stepper driver connection with ESP32


#define EN_PIN    22  // Nano v3:  16 Mega:  38  //enable (CFG6)
#define DIR_PIN   19//6  //      19      55  //direction
#define STEP_PIN  23//7  //     18      54  //step

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
