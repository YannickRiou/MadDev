// ESP32 Support Library
#include <WiFi.h>
#include <HTTPClient.h>

#include "stepper.h"
#include "ble.h"
#include "wifiCom.h"

// Main setup
void setup()
{
  // Enable serial for debug only
  Serial.begin(115200);
  delay(5000);

  //wifiInit();
  BLE_init();
  stepperInit();
  stepperEnable();
}

// Main loop
void loop()
{

  //if (playPause == "1")
  {
    if(Dir=="1")
    {
      digitalWrite(DIR_PIN, HIGH);
    }
    else
    {
       digitalWrite(DIR_PIN, LOW);
    }
    stepperEnable();
    for (int i = 0; i < 40; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(300); 
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(300); 
    }
    stepperDisable();
  }
  
  

}
