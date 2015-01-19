import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import hypermedia.net.*; 
import processing.serial.*; 
import ddf.minim.analysis.*; 
import ddf.minim.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Beat_Detection extends PApplet {

/**
Some comment
*/


Serial arduino;




Minim minim;
AudioInput in;
FFT fft;

// Visualizer efaults
float valScale = 1.5f;
float maxVisible = 10.0f;
float beatThreshold = 0.30f;
float colorOffset = 30;
float autoColorOffset = 0.01f;
float colorBrightness = 1.0f;
boolean beatDetectionOn = true; // On/off

// Show text if recently adjusted
boolean showscale = false;
boolean showBeatThreshold = false;
boolean showHelp = false;
boolean showColorBrightness = false;
boolean showBeatDetectionState = false;

float beatH = 0;
float beatS = 0;
float beatB = 0;
float arduinoBeatB = 0;

float[] lastY;
float[] lastVal;

int buffer_size = 1024;  // also sets FFT size (frequency resolution)
float sample_rate = 44100;

int redPin = 5;
int greenPin = 6;
int bluePin = 3;

boolean fullscreen = false;
int lastWidth = 0;
int lastHeight = 0;

boolean arduinoConnected = false;
int arduinoIndex = 0;
String arduinoMessage = "";


UDP udp;  // define the UDP object
Led[] leds = new Led[1200];
String ip       = "131.215.166.100";  // the remote IP address
int port        = 8888;    // the destination port
boolean sendCompleted = true;

public void setup() {

  size(500, 300);
  frame.setResizable(true);
  
  background(0);
  
  minim = new Minim(this);
  in = minim.getLineIn(Minim.MONO,buffer_size,sample_rate);
  
  fft = new FFT(in.bufferSize(), in.sampleRate());
  fft.logAverages(16, 2);
  fft.window(FFT.HAMMING);
  
  lastY = new float[fft.avgSize()];
  lastVal = new float[fft.avgSize()];
  initLasts();
  
  textSize(10);
  
  frame.setAlwaysOnTop(true);
  
  udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
  udp.log( true );     // <-- printout the connection activity
  udp.listen( true );           // and wait for incoming message
  
  for(int i = 0; i < 1200; i++) {
    leds[i] = new Led();
  }
  
  initArduino();
}

public int leftBorder()   { return PApplet.parseInt(.05f * width); }
public int rightBorder()  { return PApplet.parseInt(.05f * width); }
public int bottomBorder() { return PApplet.parseInt(.05f * width); }
public int topBorder()    { return PApplet.parseInt(.05f * width); }

public void initArduino()
{
  sendRefresh();
}

public void initLasts()
{
  
  for(int i = 0; i < fft.avgSize(); i++) {
    lastY[i] = height - bottomBorder();
    lastVal[i] = 0;
  }
  
}

public void draw() {
   
    colorMode(RGB);
  
    // Detect resizes
    if(width != lastWidth || height != lastHeight)
    {
      lastWidth = width;
      lastHeight = height;
      background(0);
      initLasts();
      println("resized");
    }
  
    // Slowly erase the screen
    fill(0,10 * 60/frameRate); // Based on 60fps
    rect(0,0,width,height - 0.8f*bottomBorder());
  
    colorMode(HSB, 100);
  
    fft.forward(in.mix);
    smooth();
    noStroke();
    
    
    int iCount = fft.avgSize();
    float barHeight =  0.03f*(height-topBorder()-bottomBorder());
    float barWidth = (width-leftBorder()-rightBorder())/iCount;
    
    float biggestValChange = 0;
    
    for(int i = 0; i < iCount; i++) {
      
      float iPercent = 1.0f*i/iCount;
      
      float highFreqscale = 1.0f + pow(iPercent, 4) * 2.0f;
      
      float val = sqrt(fft.getAvg(i)) * valScale * highFreqscale / maxVisible;
      
      float y = height - bottomBorder() - val * (height - bottomBorder() - topBorder());
      float x = leftBorder() + iPercent * (width - leftBorder() - rightBorder()) ;
      
      float h = 100 - (100.0f * iPercent + colorOffset) % 100;
      float s = 70 - pow(val, 3) * 70;
      float b = 100;
      
      if(beatDetectionOn)
      {
        fill(h, s, b);
        textAlign(CENTER, BOTTOM);
        text(nf(PApplet.parseInt(100*val),2), x+barWidth/2, y);
      }
           
      rectMode(CORNERS);
      rect(x, y+barHeight/2, x+barWidth, lastY[i]+barHeight/2);
      
      float valDiff = val - lastVal[i];
      if(valDiff > beatThreshold && valDiff > biggestValChange)
      {
        biggestValChange = valDiff;
        beatH = h;
        beatS = s;
        beatB = b;
      }
      
      lastY[i] = y;
      lastVal[i] = val;

    }
    
    // If we've hit a beat, bring the brightness of the bar up to full
    if(biggestValChange > beatThreshold)
    {
      arduinoBeatB = 100;
    }  
    
    // calculate the arduino beat color
    int c_hsb = color(beatH, 90, constrain(arduinoBeatB, 1, 100));
    
    int r = PApplet.parseInt(red(c_hsb) / 100 * 255 * colorBrightness);
    int g = PApplet.parseInt(green(c_hsb) / 100 * 255 * colorBrightness);
    int b = PApplet.parseInt(blue(c_hsb) / 100 * 255 * colorBrightness);
   
    // clear out the message area
    fill(0);
    rect(0, height - 0.8f*bottomBorder(), width, height);
    
    // draw the beat bar
    colorMode(RGB, 255);
    fill(r, g, b);
    rect(leftBorder(), height - 0.8f*bottomBorder(), width-rightBorder(), height - .5f*bottomBorder());

    // Tell the arduino to draw
    if (arduinoConnected)
    {
      try
      {
        
        if(beatDetectionOn && sendCompleted) {
          
          for(int i = 0; i < 1200; i++) {
            leds[i].r = (byte)r;
            leds[i].g = (byte)g;
            leds[i].b = (byte)b;
          }
          
          sendUpdate();
          
        }
        
        fill(16,16,16);
        textAlign(CENTER, BOTTOM);
        text(arduinoMessage, width/2, height);
      }
      catch (Exception e) {
        arduinoConnected = false;
        arduinoMessage = "Lost connection!  Press TAB to reconnect.";
        arduinoIndex--; // Pressing TAB advances, but we want to retry the same index
        println(e);
      }
    }
    else
    {
      fill(16);
      rect(0, topBorder()-15, width, topBorder()+15);
      
      fill(255,64,64);
      textAlign(CENTER, CENTER);
      text("Arduino error: " + arduinoMessage, width/2, topBorder());
    }

    // Decay the arduino beat brightness (based on 60 fps)
    //arduinoBeatB *= 1.0 - 0.10 * 60/frameRate;
    arduinoBeatB *= 1.0f - 0.08f * 60/frameRate;
    
    // Automatically advance the color
    colorOffset += autoColorOffset;
    colorOffset %= 100;

    // Show the scale if it was adjusted recently
    if(showscale)
    {
      fill(255,255,255);
      textAlign(RIGHT, TOP);
      text("scale:"+nf(valScale,1,1), width-rightBorder(), topBorder());
      showscale = false;
    }
    
    // Show the beat threshold if it was adjusted recently
    if(showBeatThreshold)
    {
      fill(255,255,255);
      textAlign(RIGHT, TOP);
      text("beat threshold:"+nf(beatThreshold,1,2), width-rightBorder(), topBorder());
      showBeatThreshold = false;
    }
    
    // Show the brightness level if adjusted recently
    if(showColorBrightness)
    {
      fill(255,255,255);
      textAlign(RIGHT, TOP);
      text("brightness level:"+nf(colorBrightness,1,2),width-rightBorder(), topBorder());
      showColorBrightness = false;
    }
    
    // Show beat detection state if adjusted recently
    if(showBeatDetectionState)
    {
      fill(255,255,255);
      textAlign(RIGHT, TOP);
      text("Beat Detection:"+(beatDetectionOn? "ON":"OFF"),width-rightBorder(), topBorder());
      showBeatDetectionState = false;
    }
     
    // Show the help
    if(showHelp)
    {
      fill(255,255,255);
      textAlign(RIGHT, TOP);
      text("Help:\nUP/DOWN arrows = Scale Visualizer\n" + 
           "LEFT/RIGHT arrows = Temporarily shift colors\n" + 
           "+/- = Beat Detection Sensitivity\n" + 
           "[/] = Adjust Brightness Level\n" +
           "TAB = Use Next Arduino Port\n" + 
           "SPACE = Toggle full-screen\n" + 
           "ENTER/RETURN = Toggle Beat Detection\n" +
           "Anything Else = Show this help", width-rightBorder(), topBorder());
      showHelp=false;
    }
     
    // Display the frame rate
    fill(16, 16, 16);
    textAlign(RIGHT, BOTTOM);
    text(nf(frameRate,2,1) + " fps", width - rightBorder(), topBorder());
    if(!fullscreen)
    {
    frame.setTitle("Beat Detection ("+nf(frameRate,2,1)+" fps)");
    }

}

public void keyReleased()
{
  if (key == CODED)
  {
   if (keyCode == UP)
   {
     valScale += 0.1f;
     showscale=true;
   }
   else if (keyCode == DOWN)
   {
     valScale -= 0.1f;
     showscale = true;
   }
   else if (keyCode == RIGHT)
   {
     colorOffset -= 5;
   }
   else if (keyCode == LEFT)
   {
     colorOffset += 5;
   }
  }
  else
  {
    if (key == '+')
    {
      beatThreshold += 0.05f;
      showBeatThreshold = true;
    }
    else if (key == '-')
    {
      beatThreshold -= 0.05f;
      showBeatThreshold = true;
    }
    else if (key == ' ')
    {
      toggleFullScreen();
    }
    else if (key == TAB)
    {
      arduinoIndex++;
      initArduino(); 
    }
    else if (key == '[')
    {
      if(colorBrightness > 0) colorBrightness -= 0.05f;
      showColorBrightness = true;
    }
    else if (key == ']')
    {
      if(colorBrightness < 1.0f) colorBrightness += 0.05f;
      showColorBrightness = true;
    }
    else if (key == ENTER || key == RETURN)
    {
      beatDetectionOn = !beatDetectionOn;
      showBeatDetectionState = true;
    }
    else
    {
      showHelp = true;
    }
  } 
}

public void keyPressed()
{
  
  // In fullscreen mode, capture ESC for exiting full screen
  if (key == ESC)
  {
   if(fullscreen)
   {
     toggleFullScreen(); 
     key=0;
   }
  }
}

public void toggleFullScreen()
{
  fullscreen = !fullscreen;
  
  frame.removeNotify();
  frame.setUndecorated(fullscreen);
  if(fullscreen) {
    frame.setSize(displayWidth, displayHeight);
    frame.setLocation(0,0);
  }
  else
  {
    frame.setSize(500, 300);
    frame.setLocation(100,100);
  }
  frame.addNotify();
}

public void stop()
{
  // always close Minim audio classes when you finish with them
  in.close();
  minim.stop();
 
  super.stop();
}


//NETWORK FUNCTIONS
public void sendUpdate() {
  sendCompleted = false;
  sendSection1();
  
}

public void receive( byte[] data ) {       // <-- default handler
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
      sendCompleted = true;
      break;
  }
  
  arduinoConnected = true;
  
}

public void sendSection1() {
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

public void sendSection2() {
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

public void sendSection3() {
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

public void sendRefresh() {
  
  byte[] refresh = new byte[1];
  refresh[0] = 4;
  
  udp.send(refresh, ip, port );
  
}

private class Led {
  public byte r, g, b;

  public Led() {
    r = 40;
    b = 40;
    g = 40;
  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Beat_Detection" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
