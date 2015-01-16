#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

int explosions[max[6]];
int max=10;
int s=1; //Inevitable in all Erich projects 
int count=0;
int rate=50;
  
void setup() {

  //Serial.begin(9600);
  
  //Serial.println("asd");

  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);

  int div = NUM_LEDS / 12;
  int colorMultiplier = 255 / div;

  for(int i=0; i < max; i++){
    explosions[i[5]]=-1;
  }

  
  CRGB tmp;

  FastLED.show();
}



void loop() {

  
  if(random(rate)<1&&count<max){
    for(int i=0; i < max; i++){
      if(explosions[i,5]<0){
        explosions[i[0]]=random(1200);
        explosions[i[1]]=random(300);
        explosions[i[2]]=random8()/3;
        explosions[i[3]]=random8()/3;
        explosions[i[4]]=random8()/3;
        explosions[i[5]]=0;
        count++;
        break();
      }
  }
  for(int i=0; i < max; i++){
    if(explosions[i[5]]>0){
      explosions[i[5]]=explosionManager(explosions[i]);
      if(explosions[i[5]]<0){
        count--;
      }
    }
  }
  FastLED.show();
  
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
int explosionManager(int vals[7]){
  int x=vals[0];
  int w=vals[1];
  int r=vals[2];
  int g=vals[3];
  int b=vals[4];
  int mark=vals[5];
  
  
  if(mark<w){
    for(int i = mark; i < w; i++) {
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
        return i;
      }
    }
    return w+1;
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
    return w+1;
  }
  return -1;
}


