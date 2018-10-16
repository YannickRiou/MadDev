/*
Copyright 2016 German Martin (gmag11@gmail.com). All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and / or other materials
provided with the distribution.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The views and conclusions contained in the software and documentation are those of the
  authors and should not be interpreted as representing official policies, either expressed
  or implied, of German Martin
*/

/*
 Name:    NtpClient.ino
 Created: 20/08/2016
 Author:  gmag11@gmail.com
 Editor:  http://www.visualmicro.com
*/

#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 2, NEO_GRB + NEO_KHZ800);

String alarm;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
  delay(500);
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
  NTP.begin("pool.ntp.org", 1, true);
  NTP.setInterval(63);
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  digitalWrite(2, HIGH);
}


void throwAlarm();
void HTMLrequestHandler();
void sendHTMLresponse();
void setup()
{ 
  static WiFiEventHandler e1, e2;
  
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("MadBox", "licorneBrume1214");
  e1 = WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client
  e2 = WiFi.onStationModeDisconnected(onSTADisconnected);
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
      Serial.print("Time Sync error: ");
      if (ntpEvent == noResponse)
        Serial.println("NTP server not reachable");
      else if (ntpEvent == invalidAddress)
        Serial.println("Invalid NTP server address");
    }
    else {
      Serial.print("Got NTP time: ");
      Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
  });
  Serial.println("DONE");
  server.begin();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}
  WiFiClient client;
  String req;

void loop()
{
  static int last = 0;
  // Check if a client has connected

if ((millis() - last) > 1000) 
{
    last = millis();
    Serial.println(NTP.getTimeDateString());
    Serial.println(req);
    if(NTP.getTimeDateString().equals(req))
    {
      Serial.println("ALARM");
      throwAlarm();
    }
}
delay(1);

if (!client) {  //no client is currently connected
    client = server.available();  //check if there is a client available
  }
  if (client && client.available()) {  //there is a client connected and there is data available
    HTMLrequestHandler();
    sendHTMLresponse();
  }
}


void HTMLrequestHandler() {
  
  String reqrcv; 
  // Read the first line of the request
  reqrcv = client.readStringUntil('\r');
  // read the rest of the submitted request, despising
  if(reqrcv.indexOf(".ico") < 0)
  {   
    req = reqrcv;
  Serial.println(req);
  req.replace("-"," ");
  req.replace("GET /","");
  req.replace("HTTP/1.1","");
  req.replace("GET /favicon.ico HTTP/1.1\n","");
  req.trim();
  Serial.print("Rcv: ");
  Serial.println(req);
  }
  client.flush();

}


void sendHTMLresponse() {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(NTP.getTimeDateString());
  client.stop();
}

void throwAlarm()
{ 
  int i,j=0;
  Serial.println("ALARRRRM");
 
  for(uint8_t j=0; j <255; j++) {
     for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 245*j/255, 176*j/255, 72*j/255);
     }
     strip.show();
     delay(100);
  };


  

   delay(10000);
   strip.setBrightness(0);
   strip.show();
  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorFade(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
      uint8_t startR, startG, startB;
      uint32_t startColor = strip.getPixelColor(i); // get the current colour
      startB = startColor & 0xFF;
      startG = (startColor >> 8) & 0xFF;
      startR = (startColor >> 16) & 0xFF;  // separate into RGB components

      if ((startR != r) || (startG != g) || (startB != b)){  // while the curr color is not yet the target color
        if (startR < r) startR++; else if (startR > r) startR--;  // increment or decrement the old color values
        if (startG < g) startG++; else if (startG > g) startG--;
        if (startB < b) startB++; else if (startB > b) startB--;
        strip.setPixelColor(i, startR, startG, startB);  // set the color
        strip.show();
        // delay(1);  // add a delay if its too fast
      }
      delay(1000);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);

}

