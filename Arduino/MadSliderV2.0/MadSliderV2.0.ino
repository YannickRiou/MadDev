// ESP32 Support Library
#include <WiFi.h>
#include <HTTPClient.h>

// TMC2130 Stepper Driver Library
#include <TMC2130Stepper.h>
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

  /*if (playPause == "1")
  {
    if(Dir=="1")
    {
      digitalWrite(DIR_PIN, HIGH);
    }
    else
    {
       digitalWrite(DIR_PIN, LOW);
    }*/
    stepperEnable();
    for (int i = 0; i < 40; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(10);
    }
    stepperDisable();
  //}
  
  

}
