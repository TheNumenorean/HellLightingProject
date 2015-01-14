#include "FastLED.h"

#define NUM_LEDS 150

#define DATA_PIN 42

#define COL_VAL 200

CRGB leds[NUM_LEDS];
CRGB tmp[NUM_LEDS];

void setup() { 
  
  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].b = 50;
    leds[i].r = 0;
    leds[i].g = random(0, 2) * COL_VAL;
  }
  
  Serial.begin(9600);
  
  
  FastLED.show();
}

void loop() {
  
  byte before;
  byte current;
  byte after;

    /* Initialize pointers */
  current = leds[0].g;
  after = leds[1].g;

  /* Test first spot */
  if (!current && after)
      tmp[0] = COL_VAL;
  else
      tmp[0] = 0;

  /* test intermediate spots */
  for (int i = 1; i < NUM_LEDS - 1; i++) {
    
    before = leds[i - 1].g;
    current = leds[i].g;
    after = leds[i + 1].g;
    
    if (!current && (!after || !before) && !(!after && !before))
      tmp[i].g = COL_VAL;
    else
        tmp[i].g = 0;
  }
    
  before = leds[NUM_LEDS - 2].g;
  current = leds[NUM_LEDS - 1].g;

  /* Test last spot */
  if (before && !current)
      tmp[NUM_LEDS - 1].g = COL_VAL;
  else
      tmp[NUM_LEDS - 1].g = 0;

  /* Move tmp to current */
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].g = tmp[i].g;
      Serial.print(tmp[i].g > 0 ? "#" : "=");
  }
  
  Serial.println();

  FastLED.show();
  delay(100);
}
