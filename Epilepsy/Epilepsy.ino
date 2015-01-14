#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 600

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB next;

void setup() { 
  
  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS);
  
  byte r, g, b;
  
  r = random(0, 255);
  g = random(0, 255);
  b = random(0, 255);
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].b = b;
    leds[i].r = r;
    leds[i].g = g;
    
  }
  
  next = CRGB::Green;
  
  
  FastLED.show();
}

void loop() {
  
  byte r, g, b;
  
  /*
  r = random(0, 255);
  g = random(0, 255);
  b = random(0, 255);
  
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].b = b;
    leds[i].r = r;
    leds[i].g = g;
    
  }
  
  */
  
  if(next.g != 0)
    next = CRGB::Purple;
  else
    next = CRGB::Green;
  
  for(int i = 0; i < NUM_LEDS; i++){
    
    leds[i] = next;
  }
  
  FastLED.show();
  
  delay(40);
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
    
  }
  
  FastLED.show();
  
  delay(80);
  
}
