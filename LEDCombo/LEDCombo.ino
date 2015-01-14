#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN_1 42
#define DATA_PIN_2 40

#define BTN_1 30
#define BTN_2 31
#define BTN_3 33

int currBtn = -1;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  
  FastLED.addLeds<APA104, DATA_PIN_1, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, DATA_PIN_2, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);
  
  
  
  
  FastLED.show();
}

void loop() {
  
  //return;
  
  int newBtn = getCurrBtn();
  
  if(newBtn != currBtn) {
    
    for (int i = 0; i < NUM_LEDS; i++){
     leds[i].r = 0;
     leds[i].g = 0;
     leds[i].b = 0;
    }
    
    FastLED.show();
    
    switch(newBtn) {
      case 0:
        fire_setup();
        break;
      case 1:
        trains_setup();
        break;
      case 2:
        wave_setup();
        break;
      case 3:
        cells_setup();
        break;
    }
    
    Serial.print(newBtn);
    Serial.print(":");
    Serial.println(currBtn);
    
    currBtn = newBtn;
    
  }
  
  Serial.println(currBtn);
  
  switch(currBtn) {
    case 0:
      fire_loop();
      break;
    case 1:
      trains_loop();
      break;
    case 2:
      wave_loop();
      break;
    case 3:
      cells_loop();
      break;
  }
  
}

int getCurrBtn() {
  
  if(digitalRead(BTN_1)) 
    return 1;
  if(digitalRead(BTN_2))
    return 2;
  if(digitalRead(BTN_3))
    return 3;
  return 0;
  
}
  

////////////////////////////////////////////////////////
//Fire

void fire_setup(){
  
  for (int i = 0; i < NUM_LEDS; i++){ // the essence of fire
     leds[i].r = random(100, 255);
     leds[i].g = random(0, 50);
     leds[i].b = 0;
  }
  
}

void fire_loop(){
  for (int i = 0; i < NUM_LEDS; i++){
     
     if(i != 0 && i != NUM_LEDS - 1){
       leds[i].r = getCloser(leds[i - 1].r, leds[i].r, leds[i + 1].r)+random(100, 255);
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

////////////////////////////////////////////////
//Trains

#define DISPLAY_LENGTH 300
#define NUM_TRAINS 5



struct train {
	float		mass;
	CRGB		color;
	float		position;
	float		velocity;
};

train trains[NUM_TRAINS];
float interval = 0.001;

void trains_setup() {
  trains[0].mass = random(1, 10);
  trains[0].color = CRGB(255,0,0);
  trains[0].position = 30;
  trains[0].velocity = 1400;
  
  trains[1].mass = random(1, 10);
  trains[1].color = CRGB(0,255,0);
  trains[1].position = 10;
  trains[1].velocity = 200;
  
  trains[2].mass = random(1, 10);
  trains[2].color = CRGB(0,0,255);
  trains[2].position = 50;
  trains[2].velocity = -400;
  
  trains[3].mass = random(1, 10);
  trains[3].color = CRGB(255,255,0);
  trains[3].position = 70;
  trains[3].velocity = 500;
  
  trains[4].mass = random(1, 10);
  trains[4].color = CRGB(255,255,255);
  trains[4].position = 90;
  trains[4].velocity = -800;
}

void trains_loop() {
  
  updateTrains(interval);
  drawTrains(true);
  FastLED.show();
  delay(interval*1000);
  
}

/*
* This function does a physical step of the train collisions.
* The positions of the trains are calculated based on their velocities, collisions and the time interval
* timeInterval - The step in time that this function will be calculating
*/
void updateTrains(float timeInterval) {
	for (int i = 0; i < NUM_TRAINS; i++) {
		train* train1 = &trains[i];
		float futurePos1 = train1->position + train1->velocity*timeInterval; // Calculate the position this train would end up in if no collisions occur
		for (int j = i+1; j < NUM_TRAINS; j++) {  // For each possible set of trains, check for collisions
			train* train2 = &trains[j];
			float futurePos2 = train2->position + train2->velocity*timeInterval;
			if ((futurePos2 >= futurePos1 && futurePos2 <= futurePos1 + train1->mass) ||
				(futurePos1 >= futurePos2 && futurePos1 <= futurePos2 + train2->mass)) { // A collision occurs if one train's would-be position is within the length of another train's would-be position
				float v1 = train1->velocity;
				float v2 = train2->velocity;
				train1->velocity = (train1->mass*v1 - train2->mass*v1 + 2 * train2->mass*v2) / (train1->mass + train2->mass); // Fancy maths for elastic collisions
				train2->velocity = (2 * train1->mass*v1 - train1->mass*v2 + train2->mass*v2) / (train1->mass + train2->mass);

				// Update train1 position for any simultaneous collisions
				futurePos1 = train1->position + train1->velocity*timeInterval;

			}
		}
		if (futurePos1 <= 0 || futurePos1 + train1->mass >= DISPLAY_LENGTH) { // The side of the track has been hit
			train1->velocity *= -1;
			futurePos1 = train1->position + train1->velocity*timeInterval;
		}
		// Train1 has final velocity now so position can be recorded
		train1->position = futurePos1;
	}
}

/*
* This function takes the list of trains and determines which pixels they should fill
* antialiasing - If true, pixels will light partially as trains approach. If false, pixels will light when half-filled by a train.
*/
void drawTrains(boolean antialiasing) {
	for (int i = 0; i < DISPLAY_LENGTH; i++) {
		leds[i].r = 0; leds[i].g = 0; leds[i].b = 0;
		for (int j = 0; j < NUM_TRAINS; j++) {
			train* train = &trains[j];
			float area = train->mass - ((max(train->position + train->mass, i + 1) - (i + 1)) - (max(train->position, i + 1) - (i + 1))) - (min(train->position + train->mass, i) - min(train->position,i));
			if (antialiasing) {
				leds[i].r += area*train->color.r;
				leds[i].g += area*train->color.g;
				leds[i].b += area*train->color.b;
			}
			else if (!antialiasing && area > .5) {
				leds[i].r = train->color.r;
				leds[i].g = train->color.g;
				leds[i].b = train->color.b;
			}
		}
	}
}

//////////////////////////////////////////////
//Wave

void wave_setup() {
  
  int div = NUM_LEDS / 6;
  int colorMultiplier = 255 / div;
  
  for(int i = 0; i < div; i++){
    leds[i].b = 1 + i * colorMultiplier;
    leds[2 * div + i].g = 4 + i * colorMultiplier;
    
    leds[4 * div + i].b = 4 + i * colorMultiplier;
    leds[4 * div + i].r = 4 + i * colorMultiplier;
    
    leds[2 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[4 * div - 1 - i].g = 4 + i * colorMultiplier;
    
    leds[6 * div - 1 - i].b = 4 + i * colorMultiplier;
    leds[6 * div - 1 - i].r = 4 + i * colorMultiplier;
  }
  
}

void wave_loop() {
  
  CRGB tmp = leds[0];
  
  for(int i = NUM_LEDS - 1; i > 0; i--) {
    CRGB tmp2 = leds[i];
    leds[i] = tmp;
    tmp = tmp2;
  }

  leds[0] = tmp;
  
  FastLED.show();
  
  delay(0);
        
}

/////////////////////////////////////////////
//Cells

#define COL_VAL 200

CRGB tmp[NUM_LEDS];

void cells_setup() {
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].b = 50;
    leds[i].r = 0;
    leds[i].g = random(0, 2) * COL_VAL;
  }
  
  //leds[NUM_LEDS/2].g = COL_VAL;
  
  FastLED.show();
  
}

void cells_loop() {
  
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
  delay(50);
}
