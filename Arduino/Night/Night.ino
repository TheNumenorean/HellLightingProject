#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 600

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  
  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS);
  
  
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = 0;
  }
  
  
  
  FastLED.show();
}

void loop() {
  FastLED.show();
  delay(1000);
}
