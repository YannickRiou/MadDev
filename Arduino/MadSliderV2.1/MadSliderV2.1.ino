// ESP32 Support Library
#include <Arduino.h>Votre position

#include <WiFi.h>

#include <HTTPClient.h>

#include "stepper.h"
#include "ble.h"
#include "wifiCom.h"

// Main setup
void setup()
{
  // Enable serial for debug only
  //Serial.begin(115200);
  delay(1000);

  wifiInit();
  //BLE_init();
  stepperInit();
  stepperEnable();
}
bool motDir = true;
// Main loop
void loop()
{

  if (playPause == "1")
  {
   
   
    for (int i = 0; i < 40; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(300); 
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(300); 
    }
    if(motDir)
    {
      digitalWrite(DIR_PIN,HIGH);
    }
    else
    {
       digitalWrite(DIR_PIN,HIGH);
    }
    motDir=!motDir;
  }
  
  

}
