/*
  Basic bike lights controlled with switches
  V1.0
  Fore White, Aft Red, Port & Starboard flashing Organge.
  14 January 2019
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    3 //strip connected to pin 3
#define LED_COUNT 150 //150 LEDs in pin

// Declare strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); 

// These constants won't change. 
// Setting toggle pin numbers:
const int toggleFore = 9;     //Pin for Fore
const int toggleAft = 10;     //Pin for Aft
const int toggleStar = 11;    //Pin for Star
const int togglePort = 12;    //Pin for port 

//Declaring default values for the the toggles
int fore = 0;                 
int aft = 0;
int port = 0;
int star = 0;

//Setup runs once
void setup() {
// initialize the toggle pins as an input:
  pinMode(toggleFore, INPUT_PULLUP);
  pinMode(toggleAft, INPUT_PULLUP);
  pinMode(toggleStar, INPUT_PULLUP);
  pinMode(togglePort, INPUT_PULLUP);
  
//Initialize strip
  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

//Loop runs constantly
void loop() {
  if (digitalRead(toggleFore) ==LOW) {
    fore = 1;}
  else{ fore = 0;}
  
  if (digitalRead(toggleAft) ==LOW) {
    aft = 1;}
  else{ aft = 0;}
  
  if (digitalRead(toggleStar) ==LOW) {
    star = 1;}
  else{ star = 0;}
  
    if (digitalRead(togglePort) ==LOW) {
    port = 1;}
  else{ port = 0;}
  
//set pixel colours. HIGH results in '0' value, so multiply desired RBG value by 1 or 0 to turn section on or off
//strip.fill(strip.Color(r,g,b), first pixel, number of pixels to count from first pixel
  strip.fill(strip.Color(fore*255,fore*255,fore*255),55,35);
  strip.fill(strip.Color(255*aft,0,0),0,18); 
  strip.fill(strip.Color(255*aft,0,0),128,18);
  strip.fill(strip.Color(star*255,star*69,0),91,36);
  strip.fill(strip.Color(port*255,port*69,0),18,36);
  
  
  strip.show();
  delay(300);
  
//set port & starboard sections to off for flash
  strip.fill(strip.Color(0,0,0),91,36);
  strip.fill(strip.Color(0,0,0),18,36);
  strip.show();
  delay(150);
}  
  
