#include "FastLED.h"

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

// How many leds in your strip?
#define NUM_LEDS 1200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN_1 7
#define DATA_PIN_2 6
#define POT_PIN A10

// Define the array of leds
CRGB leds[NUM_LEDS];

byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
unsigned int localPort = 8888; 

EthernetUDP Udp;

void setup() {

  Serial.begin(9600);
  
  Serial.println("Running");
  
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  FastLED.addLeds<APA104, DATA_PIN_1, GRB>(leds, 0, NUM_LEDS / 2);
  FastLED.addLeds<APA104, DATA_PIN_2, GRB>(leds, NUM_LEDS / 2, NUM_LEDS / 2);
  FastLED.show();
}

void loop() {
  
  int packetSize = Udp.parsePacket();
  Serial.println(packetSize);
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read((char*)leds, NUM_LEDS * 3);

/*
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    */
  }

  FastLED.show();
}


