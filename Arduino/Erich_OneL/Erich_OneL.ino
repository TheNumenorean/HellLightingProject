#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define   DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {

  Serial.begin(9600);
  
  Serial.println("asd");

  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);

  int div = NUM_LEDS / 36;
  int colorMultiplier = 255 / div;

  for(int i = 0; i < div; i++){
    leds[i].b = 4 + i * colorMultiplier;
    leds[i].r = 4 + i * colorMultiplier;
    leds[i].g = 4 + i * colorMultiplier;

    leds[2 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[2 * div - 1 - i].r = 4 + i * colorMultiplier;
    leds[2 * div - 1 - i].g = 4 + i * colorMultiplier;
    
  }

  
  CRGB tmp;

  for(int i = NUM_LEDS / 2; i < NUM_LEDS * 3 / 4; i++) {
    tmp = leds[i];
    leds[i] = leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i];
    leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i] = tmp;
  }
  
  

  FastLED.show();
}

int count=0;
byte fo=1;

void loop() {
  
  if(count >600){
    fo=0;  
  }
  if(count < 2){
    fo=1;  
  }  
  
  if(fo){
    count++;
  } else {
    count--;
  }

  int s = count; //Speed 
  CRGB tmp1[s], tmp2[s];
  
  for(int i = 0; i < s; i++){
    tmp1[i] = leds[i];
    tmp2[i] = leds[NUM_LEDS - 1 - i];
  }

  for(int i = 0; i < NUM_LEDS / 2 - s; i++) {
    leds[i] = leds[i + s];
  }
  

  for(int i = NUM_LEDS - 1; i > NUM_LEDS / 2 - s + 1 ; i--) {
    leds[i] = leds[i - s];
  }
  for(int i = 0; i < s; i++){
    leds[NUM_LEDS / 2 - 1 + s - i] = tmp1[i];
    leds[NUM_LEDS / 2 - s + i] = tmp2[i];
  }

  FastLED.show();
}


