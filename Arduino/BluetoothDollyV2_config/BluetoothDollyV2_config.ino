/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int stepPin = 12; 
const int dirPin = 13; 

const int MS1 = 2; 
const int MS2 = 3;
const int MS3 = 4 ; 

const int FDC = 10;

 int flag = 0;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  pinMode(FDC,INPUT_PULLUP);

  pinMode(MS1,OUTPUT); 
  pinMode(MS2,OUTPUT);
  pinMode(MS3,OUTPUT); 

  digitalWrite(MS1,LOW);
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,LOW);

  Serial.begin(9600);
  Serial.println("HELLO FROM DOLLY");
}
void loop() {


    if(!digitalRead(FDC))
    {
          digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
          flag =1;
    }
    else if (digitalRead(FDC) && flag == 0)
    {
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
    }
    // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 100; x++) {
      digitalWrite(stepPin,HIGH); 
      delay(10); 
      digitalWrite(stepPin,LOW); 
      delay(10); 
    }

 
  
 
}
