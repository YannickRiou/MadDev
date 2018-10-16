#include <AccelStepper.h>


// Define a stepper and the pins it will use

AccelStepper stepper(1, 8, 9);


int pos = 570;


void setup()

{  

  stepper.setMaxSpeed(40);


}


void loop()

{

  if (stepper.distanceToGo() == 0)

  {

    delay(500);

    pos = -pos;

    stepper.moveTo(pos);

  }

  stepper.run();

}
