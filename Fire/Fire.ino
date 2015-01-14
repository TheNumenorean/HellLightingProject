#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 150

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 42

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  
  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  for (int i = 0; i < NUM_LEDS; i++){ // the essence of fire
     leds[i].r = random(100, 255);
     leds[i].g = random(0, 50);
     leds[i].b = 0;
  }
  
  
  FastLED.show();
}

void loop() {
      
      for (int i = 0; i < NUM_LEDS; i++){
        
        if(i != 0 && i != NUM_LEDS - 1){
          leds[i].r = qadd8(getCloser(leds[i - 1].r, leds[i].r, leds[i + 1].r), random(100, 255));
          leds[i].g = random(0, 50);//getCloser(leds[i - 1].g, leds[i].g, leds[i + 1].g) + 
          leds[i].b = 0;
        }
      }
                  
        FastLED.show();

	delay(25);
}

int getCloser(byte a, byte b, byte c){
  
  int x = (a - b) + (c - b);
  
  return x / 4;
  
}
