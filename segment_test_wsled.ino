#include <Adafruit_NeoPixel.h>
#define PIN 13
#define NUMPIXELS 95

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 pixels.begin();
 Serial.begin(115200);
}

void loop() {
  //segment blink code 
  for(int i=1;i<29;i++){
    for(int j=3*(i-1);j<(3*i);j++){
       pixels.setPixelColor(j, pixels.Color(255,0,0));
    }
    delay(100);  
    pixels.show();
    delay(500);
    for(int j=3*(i-1);j<(3*i);j++){
       pixels.setPixelColor(j, pixels.Color(0,0,0));
    }
    delay(100);  
    pixels.show();
    }
  for(int i=84;i<95;i++){
    pixels.setPixelColor(i, pixels.Color(255,0,0));
    delay(100);  
    pixels.show();
    delay(500);
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    delay(100);  
    pixels.show();
  }
//All leds RGB flash code given below
  /*
  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, pixels.Color(255,0,0));
  delay(100);  
  pixels.show();
  delay(500);
  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, pixels.Color(0,255,0));
  delay(100);    
  pixels.show();
  delay(500);
  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, pixels.Color(0,0,255));
  delay(100);  
  pixels.show();
  delay(500);
  */
}
