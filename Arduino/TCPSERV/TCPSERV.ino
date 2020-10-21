
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Hardcode WiFi parameters as this isn't going to be moving around.
const char* ssid = "MadBox";
const char* password = "licorneBrume1214";

// Start a TCP Server on port 5045
WiFiServer server(8888);
 WiFiClient client;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("");
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.print("Connected to "); Serial.println(ssid);
  //Serial.print("IP Address: "); Serial.println(WiFi.localIP());
 
  // Start the TCP server
  server.begin();
}

void loop()
{
    if (!client.connected()) {
        // try to connect to a new client
        client = server.available();
    } else {
        // read data from the connected client
        if (client.available() > 0) {
            Serial.write(client.read());

        }
    }
}

