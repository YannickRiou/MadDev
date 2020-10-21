// Define stepper driver connection with ESP32


#define EN_PIN    4  // Nano v3:  16 Mega:  38  //enable (CFG6)
#define DIR_PIN   13//6  //      19      55  //direction
#define STEP_PIN  15//7  //     18      54  //step
#define CS_PIN    0 //      17      64  //chip select

// Main variables for stepper operations
bool dir = true;

// Store the actual position of the camera on the dolly in steps
uint32_t positionInStep = 0;

// Define a new stepper object with pinout defined in stepper.h
TMC2130Stepper driver = TMC2130Stepper(EN_PIN, DIR_PIN, STEP_PIN, CS_PIN, 16, 17, 5);

void stepperInit()
{
  driver.begin();
  driver.rms_current(600);
  driver.microsteps(256);
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
