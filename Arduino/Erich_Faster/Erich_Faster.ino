#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {

  //Serial.begin(9600);
  
  //Serial.println("asd");

  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);

  int div = NUM_LEDS / 12;
  int colorMultiplier = 255 / div;

  for(int i = 0; i < div; i++){
    leds[i].b = 4 + i * colorMultiplier;
    leds[i].r = 4 + i * colorMultiplier;
    leds[2 * div + i].b = 4 + i * colorMultiplier;
    leds[4 * div + i].g = 4 + i * colorMultiplier;
    leds[4 * div + i].b = 4 + i * colorMultiplier;
    leds[6 * div + i].g = 4 + i * colorMultiplier;
    leds[8 * div + i].r = 4 + i * colorMultiplier;
    leds[8 * div + i].g = 4 + i * colorMultiplier;
    leds[10 * div + i].r = 4 + i * colorMultiplier;

    leds[2 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[2 * div - 1 - i].r = 4 + i * colorMultiplier;
    leds[4 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[6 * div - 1 - i].g = 4 + i * colorMultiplier;
    leds[6 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[8 * div - 1 - i].g = 4 + i * colorMultiplier;
    leds[10 * div - 1 - i].r = 4 + i * colorMultiplier;
    leds[10 * div - 1 - i].g = 4 + i * colorMultiplier;
    leds[12 * div - 1 - i].r = 4 + i * colorMultiplier;
  }

  
  CRGB tmp;

  for(int i = NUM_LEDS / 2; i < NUM_LEDS * 3 / 4; i++) {
    tmp = leds[i];
    leds[i] = leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i];
    leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i] = tmp;
  }
  
  

  FastLED.show();
}

void loop() {

  CRGB tmp1, tmp2, tmp3, tmp4;

  tmp1 = leds[0];
  tmp3 = leds[1];

  for(int i = 0; i < NUM_LEDS / 2 - 2; i++) {
    leds[i] = leds[i + 2];
  }
  
  tmp2 = leds[NUM_LEDS - 1];
  tmp4 = leds[NUM_LEDS - 2];

  for(int i = NUM_LEDS - 1; i > NUM_LEDS / 2 - 1 ; i--) {
    leds[i] = leds[i - 2];
  }
  
  leds[NUM_LEDS / 2] = tmp3;
  leds[NUM_LEDS / 2 - 1] = tmp4;
  

  leds[NUM_LEDS / 2 - 2] = tmp2;
  leds[NUM_LEDS / 2 + 1] = tmp1;

  FastLED.show();
}


