// Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
 UDP udp;  // define the UDP object
 byte current = 0;
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 udp.log( true );     // <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 }
 
 void draw()
 {
 }
 
 void keyPressed() {
   
   println("Sending");
 String ip       = "192.168.1.177";  // the remote IP address
 int port        = 8888;    // the destination port
 
 byte[] data = new byte[1200];
 
 current += 10;
 
 for(int i = 0; i < 1200; i++) {
   data[i] = current;
 }
 
 println(udp.send(data, ip, port ));   // the message to send
 
 }
 
 void receive( byte[] data ) {       // <-- default handler
 //void receive( byte[] data, String ip, int port ) {  // <-- extended handler
 
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 }
