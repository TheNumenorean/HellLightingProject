#define NO_CORRECTION 1
#include <FastLED.h>
#define DISPLAY_LENGTH 150
#define NUM_TRAINS 5



CRGB disp[DISPLAY_LENGTH];

struct train {
	float		mass;
	CRGB		color;
	float		position;
	float		velocity;
};

train trains[NUM_TRAINS];
float interval = 0.001;

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
		disp[i].r = 0; disp[i].g = 0; disp[i].b = 0;
		for (int j = 0; j < NUM_TRAINS; j++) {
			train* train = &trains[j];
			float area = train->mass - ((max(train->position + train->mass, i + 1) - (i + 1)) - (max(train->position, i + 1) - (i + 1))) - (min(train->position + train->mass, i) - min(train->position,i));
			if (antialiasing) {
				disp[i].r += area*train->color.r;
				disp[i].g += area*train->color.g;
				disp[i].b += area*train->color.b;
			}
			else if (!antialiasing && area > .5) {
				disp[i].r = train->color.r;
				disp[i].g = train->color.g;
				disp[i].b = train->color.b;
			}
		}
	}
}

void setup() {
  FastLED.addLeds<APA102, 42, GRB>(disp, DISPLAY_LENGTH);
  trains[0].mass = 1;
  trains[0].color = CRGB(255,0,0);
  trains[0].position = 10;
  trains[0].velocity = 1400;
  trains[1].mass = 4;
  trains[1].color = CRGB(0,255,0);
  trains[1].position = 3;
  trains[1].velocity = 200;
  trains[2].mass = 10;
  trains[2].color = CRGB(0,0,255);
  trains[2].position = 20;
  trains[2].velocity = -400;
  trains[3].mass = 3;
  trains[3].color = CRGB(255,255,0);
  trains[3].position = 70;
  trains[3].velocity = 500;
  trains[4].mass = 2;
  trains[4].color = CRGB(255,255,255);
  trains[4].position = 80;
  trains[4].velocity = -800;
}

void loop() {
  updateTrains(interval);
  drawTrains(true);
  FastLED.show();
  delay(interval*1000);
}
