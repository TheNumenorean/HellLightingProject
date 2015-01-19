// Processing UDP example to send and receive string data from Arduino 
// press any key to send the "Hello Arduino" message


import hypermedia.net.*;

UDP udp;  // define the UDP object

String ip       = "192.168.1.177";  // the remote IP address
int port        = 8888;    // the destination port

private class Led {
  public byte r, g, b;

  public Led() {
    r = 40;
    b = 40;
    g = 40;
  }
}

Led[] leds = new Led[1200];

void setup() {
  udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
  udp.log( true );     // <-- printout the connection activity
  udp.listen( true );           // and wait for incoming message
  
  for(int i = 0; i < 1200; i++) {
    leds[i] = new Led();
  }
  
}

void draw()
{
}

void keyPressed() {

  sendUpdate();
}

void sendUpdate() {
  sendSection1();
  
}

void receive( byte[] data ) {       // <-- default handler
  //void receive( byte[] data, String ip, int port ) {  // <-- extended handler

  for (int i=0; i < data.length; i++) 
    print(data[i]);  
  println();
  
  switch(data[0]) {
    case 1:
      sendSection2();
      break;
    case 2:
      sendSection3();
      break;
    case 3:
      sendRefresh();
      break;
  }
}

void sendSection1() {
  println("Sending 1");
  byte[] data = new byte[1201];

  data[0] =  1;
  for (int i = 0; i < 400; i++) {
    data[i * 3 + 1] = leds[i].r;
    data[i * 3 + 2] = leds[i].g;
    data[i * 3 + 3] = leds[i].b;
  }
  
  udp.send(data, ip, port );
}

void sendSection2() {
  println("Sending 2");
  byte[] data = new byte[1201];

  data[0] =  2;
  for (int i = 0; i < 400; i++) {
    data[i * 3 + 1] = leds[i + 400].r;
    data[i * 3 + 2] = leds[i + 400].g;
    data[i * 3 + 3] = leds[i + 400].b;
  }

  udp.send(data, ip, port );
}

void sendSection3() {
  println("Sending 3");
  byte[] data = new byte[1201];

  data[0] =  3;
  for (int i = 0; i < 400; i++) {
    data[i * 3 + 1] = leds[i + 800].r;
    data[i * 3 + 2] = leds[i + 800].g;
    data[i * 3 + 3] = leds[i + 800].b;
  }

  udp.send(data, ip, port );
}

void sendRefresh() {
  
  byte[] refresh = new byte[1];
  refresh[0] = 4;
  
  udp.send(refresh, ip, port );
  
}



