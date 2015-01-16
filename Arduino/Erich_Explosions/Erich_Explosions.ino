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

  /*for(int i = 0; i < div; i++){
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
  }*/

  
  CRGB tmp;

  /*for(int i = NUM_LEDS / 2; i < NUM_LEDS * 3 / 4; i++) {
    tmp = leds[i];
    leds[i] = leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i];
    leds[NUM_LEDS + NUM_LEDS / 2 - 1 - i] = tmp;
  }*/
  
  

  FastLED.show();
}

void loop() {

  
  int x=random(1200);
  int w=random(300);
  int r=random8()/3;
  int g=random8()/3;
  int b=random8()/3;
  int n;
  int s=2; //Inevitable in all Erich projects
  
  set(x,r,g,b);
  FastLED.show();
    
  for(int i = 0; i < w; i++) {
    if(x + i > NUM_LEDS - 1){
      set(x + i - NUM_LEDS - 1,r,g,b);
    }
    else{
      set(x + i,r,g,b);
    }
    if(x - i < 0){
      set(x - i + NUM_LEDS,r,g,b);
    }
    else{
      set(x - i,r,g,b);
    }
    if(i%s==0){
      FastLED.show();
    }
  }
  while(r>0||b>0||g>0){
    r=fader(r);
    b=fader(b);
    g=fader(g);
    set(x,r,g,b);
    for(int i = 0; i < w; i++) {
      if(x + i > NUM_LEDS - 1){
        set(x + i - NUM_LEDS - 1,r,g,b);
      }
      else{
        set(x + i,r,g,b);
      }
      if(x - i < 0){
        set(x - i + NUM_LEDS,r,g,b);
      }
      else{
        set(x - i,r,g,b);
      }
    }
    FastLED.show();
  }


  
  
  FastLED.show();
  //delay(random8());
}

void set(int in, int R, int G, int B){
 int n;
 
  
  if(in > NUM_LEDS / 2 - 1){
      n=NUM_LEDS - (in - (NUM_LEDS / 2 - 1));
    }
    else{
      n = in;
    }  
    leds[n].b = B;
    leds[n].r = R;
    leds[n].g = G;
 
} 

int fader(int c){
  int fade=1;
  int result = c - fade;
  if(result < 0){
    return 0;
  }
  return result;
}


