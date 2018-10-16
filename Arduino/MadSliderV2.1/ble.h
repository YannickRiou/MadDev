#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <BLE2902.h>


#define SERVICE_UUID "71d22714-025b-424d-9ae4-25680cbb2be0" // UART service UUID
#define EXPO_TIME    "7fe2924b-4b9b-4262-b281-7b3c1bd054dd"
#define DELAY_TIME   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define NB_IMAGE     "1752f449-8bbc-4fd3-921a-5d0a2eacf612"
#define PLAYPAUSE    "1ac17b1f-60d1-4f90-a6e2-c7d57baa7e09"

#define STEPPER_UUID "9c944cbb-68e4-48b0-abca-91a57b2a130c"
#define CURRENT_POS  "81b4689f-bcd2-47b5-9783-09a15e9811ee"
#define DIR          "f10fa210-b70d-44f2-b20f-ce713c789b08"


BLECharacteristic *pCharEXPO_TIME;
BLECharacteristic *pCharDELAY_TIME;
BLECharacteristic *pCharNB_IMAGE;
BLECharacteristic *pCharPLAYPAUSE;
BLECharacteristic *pCharCURRENT_POS;
BLECharacteristic *pCharDIR;

String NbImage;
String delayTime;
String expo;
String playPause;
String currentPos;
String Dir;


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
   
      if(pCharacteristic == pCharEXPO_TIME)
      {
          if (rxValue.length() > 0) {
            //Serial.println("*********");
            //Serial.print("Received Value EXPO: ");
            delayTime="";
            for (int i = 0; i < rxValue.length(); i++)
              expo.concat(rxValue[i]);
            //Serial.println("*********");
            delay(50);
          }
      }
      else if(pCharacteristic == pCharDELAY_TIME)
      {
          if (rxValue.length() > 0) {
            //Serial.println("*********");
            //Serial.print("Received Value DELAY: ");
            delayTime="";
            for (int i = 0; i < rxValue.length(); i++)
                delayTime.concat(rxValue[i]);
            //Serial.println("*********");
            delay(50);
          }
      }
      else if(pCharacteristic == pCharNB_IMAGE)
      {
          if (rxValue.length() > 0) {
            //Serial.println("*********");
            //Serial.print("Received Value IMAGE: ");
            NbImage="";
            for (int i = 0; i < rxValue.length(); i++)
                NbImage.concat(rxValue[i]);
            //Serial.println("*********");
            delay(50);
          }
      }
       else if(pCharacteristic == pCharCURRENT_POS)
      {
          if (rxValue.length() > 0) {
            //Serial.println("*********");
            //Serial.print("Received Value CURRENT POS: ");
            currentPos="";
            for (int i = 0; i < rxValue.length(); i++)
                currentPos.concat(rxValue[i]);
            //Serial.println("*********");
            delay(50);
          }
      }

       else if(pCharacteristic == pCharPLAYPAUSE)
      {
          if (rxValue.length() > 0) {
            //Serial.println("*********");
            //Serial.print("Received Value PLAYPAUSE: ");
            playPause="";
            for (int i = 0; i < rxValue.length(); i++)
                playPause.concat(rxValue[i]);
            //Serial.println(playPause);
            delay(50);
          }
      }
      
      else if(pCharacteristic == pCharDIR)
      {
          if (rxValue.length() > 0) {
            Dir="";
            for (int i = 0; i < rxValue.length(); i++)
                Dir.concat(rxValue[i]);
            delay(50);
          }
      }

    }
};

void BLE_init()
{
  BLEDevice::init("MadDolly");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLEService *pServiceStepper = pServer->createService(STEPPER_UUID);

  pCharEXPO_TIME =  pService->createCharacteristic(
   EXPO_TIME,
        BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
  );

  pCharDELAY_TIME = pService->createCharacteristic(
    DELAY_TIME,
       BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
  );

  pCharNB_IMAGE = pService->createCharacteristic(
     NB_IMAGE,
       BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
   );

  pCharPLAYPAUSE = pService->createCharacteristic(
     PLAYPAUSE,
     BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
  );

  pCharCURRENT_POS = pServiceStepper->createCharacteristic(
     CURRENT_POS,
       BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
  );

   pCharDIR = pServiceStepper->createCharacteristic(
     DIR,
       BLECharacteristic::PROPERTY_WRITE|
   BLECharacteristic::PROPERTY_READ
  );

  pCharEXPO_TIME->addDescriptor(new BLE2902());
  pCharEXPO_TIME->setCallbacks(new MyCallbacks());

  pCharDELAY_TIME->addDescriptor(new BLE2902());
  pCharDELAY_TIME->setCallbacks(new MyCallbacks());

  pCharNB_IMAGE->addDescriptor(new BLE2902());
  pCharNB_IMAGE->setCallbacks(new MyCallbacks());

   pCharPLAYPAUSE->addDescriptor(new BLE2902());
  pCharPLAYPAUSE->setCallbacks(new MyCallbacks());

  pCharCURRENT_POS->addDescriptor(new BLE2902());
  pCharCURRENT_POS->setCallbacks(new MyCallbacks());

  pCharDIR->addDescriptor(new BLE2902());
  pCharDIR->setCallbacks(new MyCallbacks());


  pService->start();
  pServiceStepper->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void sendBLECmd(BLECharacteristic *pChar, uint8_t value)
{
  pChar->setValue(&value, 1);
  pChar->notify();
}

