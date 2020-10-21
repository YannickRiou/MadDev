#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Hardcode WiFi parameters as this isn't going to be moving around.
const char* ssid = "MadBox";
const char* password = "licorneBrume1214";

WiFiClient serverClients[2];

// Start a TCP Server on port 5045
WiFiServer server(5005);

unsigned long previousMillis = 0;

void setup(void) {
 Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("");
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  
  // Start the TCP server
  server.begin();
  }

void loop() {
  unsigned long currentMillis = millis();
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < 2; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }

  }

  //do every 200ms
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    for (i = 0; i < 2; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write("132");
        delay(1);
      }
    }

  }
 }
