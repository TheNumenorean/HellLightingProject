#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN_1 2
#define DATA_PIN_2 3

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  
  FastLED.addLeds<APA104, DATA_PIN_1, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, DATA_PIN_2, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);

  
  
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i].r = 255;
      leds[i].g = 255;
      leds[i].b = 255;
  }
  
  
  
  FastLED.show();
}

void loop() {
  FastLED.show();
  delay(1000);
}
