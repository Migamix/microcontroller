//see https://github.com/FastLED/FastLED/wiki/Basic-usage for use if FastLED.h
#include <FastLED.h>
#define NUM_LEDS 12 //5x8 matrix (adaFruit Neopixel)
#define DATA_PIN 6

CRGB leds[NUM_LEDS]; //sets array of LED count

void setup() {
  // put your setup code here, to run once:
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
//  FastLED.setBrightness(1); //1-255
//int shift = 0;
//int col = 0;
}

void loop() {
int shift = random(5); 
int col = shift*8;
      for(int dot = 0; dot < 8; dot++) {
         
            leds[dot+col] = 0x555555;
            FastLED.show();
            leds[dot+col] = 0x001100;
            FastLED.show();
            delay(50); //time between light changes
      }  
      for(int dot = 0; dot < 8; dot++) {
            leds[dot+col] = 0x000100;
            FastLED.show();
            delay(75);
            int rem = random(40);
            leds[rem] = 0x000200;
            FastLED.show();
            delay(15);
            leds[rem] = 0x000000;
            FastLED.show();
      }
}
