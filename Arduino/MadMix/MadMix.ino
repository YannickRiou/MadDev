#include <Adafruit_NeoPixel.h>


#include <MIDI.h>
/*
  This file is public domain. Use it as you like.
*/

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 2, NEO_GRB + NEO_KHZ800);


int ledPin = 13;
int note_on = 0;

#define BP11 48
#define BP12 52
#define BP13 46

#define BP21 50
#define BP22 44 
#define BP23 42

#define BP31 36
#define BP32 38
#define BP33 40

#define POT1 A0
#define POT2 A2
#define POT3 A1



int pad[] = {BP11,BP12,BP13,BP21,BP22,BP23,BP31,BP32,BP33};
char* padName[] = {"BP11","BP12","BP13","BP21","BP22","BP23","BP31","BP32","BP33"};
int i =0;

int pot1value = 0;
int pot2value = 0;
int pot3value = 0;

int old_pot1 = 0;
int old_pot2 = 0;
int old_pot3 = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
  Serial.begin(115200);
 
  
  for(i=0;i<9;i++)
  {
    pinMode(pad[i], INPUT);
    digitalWrite(pad[i], INPUT_PULLUP);
  }

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
colorWipe(strip.Color(255, 0, 0), 50); // Blue 
}

int statutBP11 = 0;
int statutBP12 = 0;
int statutBP13 = 0;

int statutBP21 = 0;
int statutBP22 = 0;
int statutBP23 = 0;

int statutBP31 = 0;
int statutBP32 = 0;
int statutBP33 = 0;


void loop()
{
    if(digitalRead(BP11) == LOW && statutBP11 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(10, 10, 1);
      colorWipe(strip.Color(255, 0,0), 1);  
      statutBP11 =1;
      delay(10);
    }
    if(digitalRead(BP11) == HIGH && statutBP11 ==1)
    {
      MIDI.sendNoteOff(10, 0, 1);
      statutBP11=0;
    }

      if(digitalRead(BP12) == LOW && statutBP12 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(11, 11, 2);
     colorWipe(strip.Color(255, 0,0), 1); 
      statutBP11 =1;
      statutBP12 =1;
      delay(10);
    }
    if(digitalRead(BP12) == HIGH && statutBP12 ==1)
    {
      MIDI.sendNoteOff(11, 0, 2);
      statutBP12=0;
    }

       if(digitalRead(BP13) == LOW && statutBP13 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(12, 12, 3);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP13 =1;
      delay(10);
    }
    if(digitalRead(BP13) == HIGH && statutBP13 ==1)
    {
      MIDI.sendNoteOff(12, 0, 3);
      statutBP13=0;
    }


    // 2nd ROW

    if(digitalRead(BP21) == LOW && statutBP21 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(13, 13, 4);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP21 =1;
      delay(10);
    }
    if(digitalRead(BP21) == HIGH && statutBP21 ==1)
    {
      MIDI.sendNoteOff(13, 0, 4);
      statutBP21=0;
    }

      if(digitalRead(BP22) == LOW && statutBP22 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(14, 14, 5);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP22 =1;
      delay(10);
    }
    if(digitalRead(BP22) == HIGH && statutBP22 ==1)
    {
      MIDI.sendNoteOff(14, 0, 5);
      statutBP22=0;
    }

       if(digitalRead(BP23) == LOW && statutBP23 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(15, 15, 6);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP23 =1;
      delay(10);
    }
    if(digitalRead(BP23) == HIGH && statutBP23 ==1)
    {
      MIDI.sendNoteOff(15, 0, 6);
      statutBP23=0;
    }

     // 3rdd ROW

    if(digitalRead(BP31) == LOW && statutBP31 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(16, 16, 7);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP31 =1;
      delay(10);
    }
    if(digitalRead(BP31) == HIGH && statutBP31 ==1)
    {
      MIDI.sendNoteOff(16, 0, 7);
      statutBP31=0;
    }

      if(digitalRead(BP32) == LOW && statutBP32 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(17, 17, 8);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP32 =1;
      delay(10);
    }
    if(digitalRead(BP32) == HIGH && statutBP32 ==1)
    {
      MIDI.sendNoteOff(17, 0, 8);
      statutBP32=0;
    }

       if(digitalRead(BP33) == LOW && statutBP33 == 0)
    {
      //Serial.println(padName[i]);
      MIDI.sendNoteOn(18, 18, 9);
colorWipe(strip.Color(255, 0,0), 1); 
      statutBP33 =1;
      delay(10);
    }
    if(digitalRead(BP33) == HIGH && statutBP33 ==1)
    {
      MIDI.sendNoteOff(18, 0, 9);
      statutBP33=0;
    }


    // Potentiometer
    pot1value = map(analogRead(POT1), 0, 1023, 0, 127);
    pot2value = map(analogRead(POT2), 0, 1023, 0, 127);
    pot3value = map(analogRead(POT3), 0, 1023, 0, 127);
    
    if (abs(pot1value-old_pot1) > 2){
     old_pot1 = pot1value;  // save the changed value
     colorWipe(strip.Color(pot1value, pot2value, pot3value), 1); // Blue 
      statutBP33 =1;
      MIDI.sendControlChange(1,pot1value,10);
     delay(10);
    }

    if (abs(pot2value-old_pot2) > 2){
     old_pot2 = pot2value;  // save the changed value
     colorWipe(strip.Color(pot1value, pot2value, pot3value), 1); // Blue 
     MIDI.sendControlChange(2,pot2value,11);
     delay(10);
    }

    if (abs(pot3value-old_pot3) > 2){
     old_pot3 = pot3value;  // save the changed value
 colorWipe(strip.Color(pot1value, pot2value, pot3value), 1); // Blue 
      MIDI.sendControlChange(3,pot3value,12);
     delay(10);
    }
    
    colorWipe(strip.Color(255, 255, 255), 1); // Blue 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
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
