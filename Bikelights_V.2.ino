/*
  V2 14 Jan 2020
     Bike lights controlled with switches
     Disco, Fore White, Aft Red, Port & Starboard flashing Orange.
  v2.1 17 Jan 2019
     Improved indicator fill pattern
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    3 // strip connected to pin 3
#define LED_COUNT 150 // 150 LEDs in pin

// Declare strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); 

// These constants won't change. 
// Setting toggle pin numbers:
const int toggleDisco = 8;    // pin for Disco
const int toggleFore = 9;     // Pin for Fore
const int toggleAft = 10;     // Pin for Aft
const int toggleStar = 11;    // Pin for Star
const int togglePort = 12;    // Pin for Port 

// Declaring default values for the the toggles
int fore = 0;                 
int aft = 0;
int port = 0;
int star = 0;
int disco = 0;

// Setup runs once
void setup() {
// initialize the toggle pins as an input:
  pinMode(toggleFore, INPUT_PULLUP);
  pinMode(toggleAft, INPUT_PULLUP);
  pinMode(toggleStar, INPUT_PULLUP);
  pinMode(togglePort, INPUT_PULLUP);
  pinMode(toggleDisco, INPUT_PULLUP);
  
// Initialize strip
  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

// Loop runs constantly
void loop() {

if (digitalRead(toggleDisco) == LOW){
   int firstPixelHue = 0;                                                // First pixel starts at red (hue 0)
    for(int b=0; b<3; b++) {                                             //  'b' counts from 0 to 2...
      strip.clear();                                                     //   Set all pixels in RAM to 0 (off)
      for(int c=b; c<strip.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue));             // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                                                      // Update strip with new contents
      delay(75);                                                         // Pause for a moment
      firstPixelHue += 65536 / 90;                                       // One cycle of color wheel over 90 frames
    }
}
else {
  for (int o = 0; o < 35; o ++) {                             //runs loop 35x to alllow fill effect on indicators
  if (digitalRead(toggleFore) == LOW) {
    fore = 1;}
  else{ fore = 0;}
  
  if (digitalRead(toggleAft) == LOW) {
    aft = 1;}
  else{ aft = 0;}
  
  if (digitalRead(toggleStar) == LOW) {
    star = 1;}
  else{ star = 0;}
  
    if (digitalRead(togglePort) ==LOW) {
    port = 1;}
  else{ port = 0;}
  
// set pixel colours. HIGH results in '0' value, so multiply desired RBG value by 1 or 0 to turn section on or off
// strip.fill(strip.Color(r,g,b), first pixel, number of pixels to count from first pixel
  strip.fill(strip.Color(fore*255,fore*255,fore*255),55,35);
  strip.fill(strip.Color(255*aft,0,0),0,18); 
  strip.fill(strip.Color(255*aft,0,0),128,18);
 
  strip.setPixelColor(126 -o, star*255,star*69,0);
  strip.setPixelColor(18 + o, port*255,port*69,0);
    
  strip.show();
  delay(15);
  }
 
  strip.show();
  delay(200);
  
// set port & starboard sections to off for flash
  strip.fill(strip.Color(0,0,0),91,36);
  strip.fill(strip.Color(0,0,0),18,36);
  strip.show();
  delay(150);
}  
}  
