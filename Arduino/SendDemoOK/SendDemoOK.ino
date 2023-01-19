 /*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #3  
  mySwitch.enableTransmit(3);
  
  // Optional set protocol (default is 1, will work for most outlets)
   mySwitch.setProtocol(1);

  // Optional set pulse length.
   mySwitch.setPulseLength(170);
  
  // Optional set number of transmission repetitions.
   //mySwitch.setRepeatTransmit(15);
  
}

// unsigned long mySwitchOn[] = { 24,4281651,4281795,4282115 };
// unsigned long mySwitchOff[] = { 24,4281660,4281804,4282124 };

long int val=0;
long int i=0;

void loop() {

  val = 4281000 + i;
  /* Same switch as above, but using decimal code */
  //mySwitch.send(4281651, 24);
  //mySwitch.send(4281795, 24);
  mySwitch.send(val, 24);
  delay(1);  
  //mySwitch.send(4281660, 24);
  //mySwitch.send(4281804, 24);
  //mySwitch.send(4282124, 24);
  Serial.println(val);
  i= i +1;

  //delay(1000);
}
