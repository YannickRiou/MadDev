/*
 *  This sketch sends a message to a TCP server
 *
 */

#define START 128
#define FULL 132
#define CLEAN 135 
#define MAX 136
#define SPOT 134
#define DOCK 143

#define MAD_SSID "MadBox"
#define MAD_PASSWD "licorneBrume1214"

#include <WiFi.h>
#include <WiFiMulti.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"
WiFiMulti WiFiMulti;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// Use WiFiClient class to create TCP connections
WiFiClient client;

AsyncWebServer server(80);

String CheckBoxChoice; 

#define LUNDI 1
#define MARDI 2
#define MERCREDI 3
#define JEUDI 4
#define VENDREDI 5
#define SAMEDI 6
#define DIMANCHE 0

bool matin = false;
bool apresmidi = false;

bool lundi = false;
bool mardi = false;
bool mercredi = false;
bool jeudi = false;
bool vendredi = false;
bool samedi = false;
bool dimanche = false;


const char* PARAM_INPUT_1 = "id";
const char* PARAM_INPUT_2 = "state";



// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  /*if(var == "lundi" && lundi){
    return "X";
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }*/
  return String();
}

void setup()
{
    Serial2.begin(115200);
    Serial.begin(115200);
    delay(10);

    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(MAD_SSID, MAD_PASSWD);

    delay(2000);
     while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
    }

 
     pinMode(22,OUTPUT);
    digitalWrite(22,LOW);

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);    

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/plan", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/plan.html", String(), false, processor);
    });

    server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/clear.html", String(), false, processor);
    });

    // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
 
    switch(inputMessage1.toInt())
    {
      case 1:
      case 11:
      lundi = true;
      break;
      case 2:
      case 12:
      mardi = true;
      break;
      case 3:
      case 13:
      mercredi = true;
      break;
      case 4:
      case 14:
      jeudi = true;
      break;
      case 5:
      case 15:
      vendredi = true;
      break;
      case 6:
      case 16:
      samedi = true;
      break;
      case 7:
      case 17:
      dimanche = true;
      break;
    }

    if(inputMessage1.toInt()<8)
    {
      matin = true;
      apresmidi=false;
    }
    else
    {
      matin = false;
      apresmidi=true;
    }
   
  
    request->send(200, "text/plain", "OK");
  });

    // Start server
    server.begin();
    
}
bool cleaning=false;
void roomba_clean()
{
  if(!cleaning)
  {
    Serial2.write(START);
    delay(500);
    Serial2.write(FULL);
    delay(500);
    Serial2.write(CLEAN);
    delay(500);
    cleaning = true;
  }
}

void loop()
{ 
    
    static int i = 0;
    static int last = 0;
   
    if ((millis () - last) > 2000) 
    {
       //Serial.println(millis() - last);
        last = millis ();
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo))
        {
          Serial.println("Failed to obtain time");
          return;
        }

        if(timeinfo.tm_hour == 12 || timeinfo.tm_hour == 0)
        {
          cleaning = false;
        }
        if((matin && (timeinfo.tm_hour > 10) && (matin && (timeinfo.tm_hour < 12))) || ((apresmidi && (timeinfo.tm_hour > 17)) && (apresmidi && (timeinfo.tm_hour > 17))))
        {
            if((timeinfo.tm_wday == LUNDI) && lundi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == MARDI) && mardi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == MERCREDI) && mercredi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == JEUDI) && jeudi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == VENDREDI) && vendredi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == SAMEDI) && samedi)
            {
              roomba_clean();
            }
            else if((timeinfo.tm_wday == DIMANCHE) && dimanche)
            {
              roomba_clean();
            }
        }
    }
    

   //WORKING ROOMBA SEND with tcp server sending string !!! 
   /*if (client.available() > 0) {
    //read back one line from server
    String line = client.readString();
    Serial2.write(line.toInt());
   }*/
   
}
