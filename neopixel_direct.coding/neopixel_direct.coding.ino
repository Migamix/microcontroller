//see https://github.com/FastLED/FastLED/wiki/Basic-usage for use if FastLED.h
#include <FastLED.h>
#include <colorbase.h>
#define NUM_LEDS 40 //5x8 matrix (adaFruit Neopixel)
#define DATA_PIN 6

CRGB leds[NUM_LEDS]; //sets array of LED count

void setup() {
  // put your setup code here, to run once:
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
  FastLED.setBrightness(1); //1-255
}

void loop() {
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
            leds[dot] = CRGB::Blue; //sets LED point and colour
            FastLED.show();
            leds[dot] = CRGB::Green; // clear this led for the next time around the loop
            delay(50); //time between light changes
        }
}
