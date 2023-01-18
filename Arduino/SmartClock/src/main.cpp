#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i =36;i<=53;i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }

  for(int i =38;i<=52;i+=2)
  {
    digitalWrite(i,HIGH);
  }
  

     delay(500);
    // digitalWrite(36,HIGH);
    // digitalWrite(37,HIGH);
    // digitalWrite(39,HIGH);
    // digitalWrite(41,HIGH);
    // digitalWrite(43,HIGH);
    // digitalWrite(45,HIGH);
    // digitalWrite(47,HIGH);
    // digitalWrite(49,HIGH);
    // digitalWrite(51,HIGH);
    // digitalWrite(53,HIGH);
    




}
int i = 0;
int val = 0;
void loop() {

      i = !i;

  for (int j =37;j<=53;j+=2)
    {
      digitalWrite(j,i);
      //for (int i =38;i<=52;i+=2)
      {
        digitalWrite(50,LOW);
        delay(100);
      }
      //digitalWrite(j,LOW);
    }
      digitalWrite(36,i);
  digitalWrite(50,LOW);
  delay(100);
}