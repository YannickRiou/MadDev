/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

const uint16_t port = 5005;
const char * host = "192.168.1.65"; // ip or dns

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup()
{
    Serial2.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("MadBox", "licorneBrume1214");

    delay(2000);
     while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
    }
    Serial2.write(128);
    delay(500);
    //Serial2.write(132);
    //delay(500);

  
    // Wait for connect
    while (!client.connect(host, port)) 
    {
      delay(1000);
    }
    
}


void loop()
{ 

   //WORKING ROOMBA SEND with tcp server sending string !!! 
   if (client.available() > 0) {
    //read back one line from server
    String line = client.readString();
    Serial2.write(line.toInt());
   }
   
}
