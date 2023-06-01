/*
V3 - 1/06/2023

  Built on ButtonLightsDisco V5.0
  Tidied code to use functions & avoid Pete's Dissapointment.
  
  Next:
  Change to FastLED library for added functionality
  Add sound reactive code  
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include "fix_fft.h"
#endif
#define LED_PIN    3 //strip connected to pin 3
#define LED_COUNT 134 //134 LEDs in pin

// Declare strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Setting toggle pin numbers:
const int toggleFore = 9;     //Pin for Fore
const int toggleAft = 10;     //Pin for Aft
const int toggleStar = 12;    //Pin for Star
const int togglePort = 11;    //Pin for Port 

//Turns on and off different themes
const int toggleThemed = 8;   //pin for Themed Lights 

//Rotary Selects the theme
const int rotaryRainbow = A1;              //Rotary Pin 12 O'clock 
const int rotaryDisco = A2;                //Rotary Pin 1 O'clock 
const int rotaryChase = A3;                //Rotary Pin 2 O'clock
const int rotaryFiveO= A5;              //Rotary Pin 3 O'clock
//const int rotaryBlank = A4;              //Rotary Pin 4 O'clock
//const int rotaryBlank = A6;              //Rotary Pin 5 O'clock
//const int rotaryaccel = A7;              //Rotary Pin 6 O'clock
const int rotaryWizzPop = 7;                  //Rotary Pin 7 O'clock
const int rotaryVolReact = 6;              //Rotary Pin 8 O'clock
const int rotaryTest = 4;                 //Rotary Pin 9 O'clock

//VolReact
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

float vol = 0;

void setup() {

//  On/Off toggles  
  pinMode(toggleFore, INPUT_PULLUP);
  pinMode(toggleAft, INPUT_PULLUP);
  pinMode(toggleStar, INPUT_PULLUP);
  pinMode(togglePort, INPUT_PULLUP);
  pinMode(toggleThemed, INPUT_PULLUP);
  
//  Pins on rotary switch  
  pinMode(rotaryFiveO, INPUT_PULLUP);
  pinMode(rotaryDisco, INPUT_PULLUP);
  pinMode(rotaryVolReact, INPUT_PULLUP);
  pinMode(rotaryChase, INPUT_PULLUP);
  pinMode(rotaryRainbow, INPUT_PULLUP);
  pinMode(rotaryTest, INPUT_PULLUP);
  pinMode(rotaryWizzPop, INPUT_PULLUP);
  //pinMode(rotaryBlank, INPUT_PULLUP);
  //pinMode(rotaryBlank, INPUT_PULLUP);
  //pinMode(rotaryBlank, INPUT_PULLUP);  
  
//Initialize strip
  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)

Serial.begin(115200); //For testing & feedback

}

void loop() {
  
  if (digitalRead(toggleThemed) == LOW) {
    
    if (digitalRead(rotaryDisco) == LOW){
      Disco();
    }
    
    if (digitalRead(rotaryFiveO) == LOW){
      FiveO();
    }
    
    if (digitalRead(rotaryVolReact) == LOW){
      VolReact();
    }

    if (digitalRead(rotaryRainbow) == LOW) {
      Rainbow();
    }

    if (digitalRead(rotaryChase) == LOW){
      Chase();
    }

    if (digitalRead(rotaryTest) == LOW){
      Test();         
    }
    
    if (digitalRead(rotaryWizzPop) == LOW){
      WizzPop();         
    }
  }
  
  else{
    Normal();   
  }
}  

/* Functions defined here:
Disco
Five0
VolReact
Rainbow
Chase
Test
Normal
*/

void Disco() {
  Serial.println("disco-blink");
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    for(int c=b; c<strip.numPixels(); c += 3) {
      int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
      uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
      strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
    }
    strip.show();                // Update strip with new contents
    delay(75);                 // Pause for a moment
    firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
  }
}
  
void FiveO (){
  strip.fill(strip.Color(255,255,255),52,30);          //Turns on front white    
  strip.fill(strip.Color(255,0,0),0,30);               // Turns on rear red
  strip.fill(strip.Color(255,0,0),104,30);
  strip.show();  
  
  for(int f=0; f<3; f ++) {
    strip.fill(strip.Color(255,0,0),82,22);
    strip.fill(strip.Color(0,0,255),30,22);
    strip.show();
    delay(150);
    strip.fill(strip.Color(0,0,0),82,22);
    strip.fill(strip.Color(0,0,0),30,22);
    strip.show();
    delay(100);
  }
  for(int f=0; f<3; f++){
    strip.fill(strip.Color(0,0,255),82,22);
    strip.fill(strip.Color(255,0,0),30,22);
    strip.show();
    delay(150);
    strip.fill(strip.Color(0,0,0),30,22);
    strip.fill(strip.Color(0,0,0),82,22);
    strip.show();
    delay(100);
  }
}
    
void VolReact(){
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
 
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow){
    sample = analogRead(A0);
    if (sample < 1024){  // toss out spurious readings
      if (sample > signalMax){
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin){
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  int pixel = volts * 20;
  for (int p = 0; p < pixel; p++ ){
    strip.setPixelColor(0 + p, 100,100,100);
    strip.setPixelColor(134 -p, 100 ,100,100);
    strip.show();
  }
  strip.show();
  delay(5);
  strip.fill(strip.Color(0,0,0),0,134);
  strip.show();
}
  
void Rainbow(){
  for(long firstPixelHue = 0; firstPixelHue < 1*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
  strip.show(); // Update strip with new contents
  }
}
 
void Chase(){
  for(int i = 0; i<184; i++){
    strip.fill(strip.Color(125,125,125),i,5);
    strip.fill(strip.Color(125,0,125),i-20,5);
    strip.fill(strip.Color(125,125,0),i-40,5);
    strip.fill(strip.Color(0,0,0),i-5,5);
    strip.fill(strip.Color(0,0,0),i-25,5);
    strip.fill(strip.Color(0,0,0),i-45,5);
    strip.show();
    delay(5);
  }
}

void WizzPop(){
 /* for(int j = 0; j <100000; j++){
  for(int i = 0; i<134; i++){
      strip.fill(strip.Color(255,0,0),i,i);
      strip.show();
      delay(0);
      strip.fill(strip.Color(0,0,0),0,134);
      strip.show();
  }
  }
*/
}


void Test (){
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow){
      sample = analogRead(A0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
   int LED = ((volts/5)*134)-10;
 
   Serial.println(LED);
    strip.fill(strip.Color(255,255,255),0,LED);
    strip.show();
    strip.fill(strip.Color(0,0,0),LED,134);
    strip.show();
}
}

 
void Normal(){
  //Declare default values for toggles
  int fore = 0;                 
  int aft = 0;
  int port = 0;
  int star = 0;
  
  for (int o = 0; o < 22; o ++) {                             //runs loop 'o' times to alllow fill effect on indicators
    if (digitalRead(toggleFore) == LOW) {
      fore = 1;
    }
    else{ fore = 0;}
    
      if (digitalRead(toggleAft) == LOW) {
        aft = 1;}
      else{ aft = 0;}
      
      if (digitalRead(toggleStar) == LOW) {
        star = 1;}
      else{star = 0;}
      
      if (digitalRead(togglePort) == LOW) {
        port = 1;}
      else{ port = 0;}
  
//set pixel colours. HIGH results in '0' value, so multiply desired RBG value by 1 or 0 to turn section on or off
//strip.fill(strip.Color(r,g,b), first pixel, number of pixels to count from first pixel
    strip.fill(strip.Color(fore*255,fore*255,fore*255),52,30);   ///Sets 32 pixels to white starting at 54    
    strip.fill(strip.Color(255*aft,0,0),0,30);                   // Sets 30 pixels to red starting at 0
    strip.fill(strip.Color(255*aft,0,0),104,30);                 // Sets 30 pixels to red starting at 104

//Fill effect for indicators
    strip.setPixelColor(103 -o, star*255,star*69,0);  
    strip.setPixelColor(30 + o, port*255,port*69,0);
    strip.show();
    delay(15);
  }
  delay(200);
//set port & starboard sections to off for flash
  strip.fill(strip.Color(0,0,0),82,22);
  strip.fill(strip.Color(0,0,0),30,22);
  
  strip.show();
  delay(150);
}
