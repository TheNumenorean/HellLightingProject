#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7
#define MAX 10
// Define the array of leds
CRGB leds[NUM_LEDS];

int explosions[MAX][6];

int s=1; //Inevitable in all Erich projects 
int count=0;
int rate=10;
  
void setup() {

  Serial.begin(9600);
  
  Serial.println("asd");

  FastLED.addLeds<APA104, DATA_PIN, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, 6, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);

  int div = NUM_LEDS / 12;
  int colorMultiplier = 255 / div;

  for(int i=0; i < MAX; i++){
    explosions[i][5]=-1;
  }

  
  CRGB tmp;

  FastLED.show();
}



void loop() {

  
  if(random(rate)<1&&count<MAX){
    for(int i=0; i < MAX; i++){
      if(explosions[i][5]<0){
        explosions[i][0]=random(1200);
        explosions[i][1]=random(50);
        explosions[i][2]=random8()/3;
        explosions[i][3]=random8()/3;
        explosions[i][4]=random8()/3;
        explosions[i][5]=0;
        count++;
        Serial.println("new");
        break;
      }
    }
  }
  for(int i=0; i < MAX; i++){
    if(explosions[i][5]>=0){
      explosions[i][5]=explosionManager(i);
      if(explosions[i][5]<0){
        count--;
        for(int k=0; k < 5; i++){
          explosions[i][k]=0;
        }
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
  int fade=10;
  int result = c - fade;
  if(result < 0){
    return 0;
  }
  return result;
}
int explosionManager(int j){
  
  int x=explosions[j][0];
  int w=explosions[j][1];
  int r=explosions[j][2];
  int g=explosions[j][3];
  int b=explosions[j][4];
  int mark=explosions[j][5];
  
  
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
      
        return i+1;
      }
    }
    
    return w+5;
  }
  
  while(r>0||b>0||g>0){
    //Serial.print("Before: ");
    //Serial.println(r);
    explosions[j][2]=fader(r);
    explosions[j][3]=fader(g);
    explosions[j][4]=fader(b);
    //Serial.print("After: ");
    //Serial.println(r);
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


