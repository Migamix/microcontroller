/* this example uses a external function accessed in another part of the programme
    to perform an extra blanking feature.*/
/* the current example is using the Adafruit NeoPixel Shield for Arduino - 40 RGB LED Pixel Matrix
    https://www.adafruit.com/product/1430
    and any Arduino Uno compatible
    this "animation" works only if the panel is rotated 90deg clockwize
    since the pixel counts are from left to right, and then down.
    after the rotation, pay attention to the starting of each column going from right to left
*/
#include <FastLED.h> // see https://github.com/FastLED/FastLED/wiki/Basic-usage for use if FastLED.h
#define NUM_LEDS 40 // 5x8 matrix (adaFruit Neopixel change if you have more)
#define ROWS 5 // helps tell the programme that it only has # rows to work with
#define COLUMN 8 // helps tell the programme that it only has # columns to work with
#define DATA_PIN 6 // the  pin that is being used to send data to the matrix
#define _remove 4 // how many points to remove per chunk (runs twice in blank() function during full loop)
#define _speed 80 // 50 - 80 is best

CRGB leds[NUM_LEDS]; //sets array of LED count


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // setup the available matrix using information in the #define statements
  for (int dot = 0; dot < NUM_LEDS; dot++) { // set all available pixels off,
    leds[dot] = 0x000000; // using HEX values for easier color settings, gives 24 bit colour resolution
    FastLED.show();
  }
}

void loop() {
  int col = random(ROWS) * COLUMN; // make the start of the drop on a new random row
  for (int dot = 0; dot < COLUMN; dot++) {
    leds[dot + col] = 0xFFFFFF; FastLED.show(); // head of line
    // forces the current pixel settings to show
    leds[dot + col] = 0x001100; FastLED.show(); // follow up color behind head of line
    delay(_speed/3);
  }
  blank(); // this is calling the "blank()" function (see below)
  for (int dot = 0; dot < COLUMN; dot++) {
    leds[dot + col] = 0x000100; // reduces the background colour some more to give the tail effect
    FastLED.show();
    delay(_speed);
  }
  blank(); // a little more blank, two calls of this function gives a hint of delay time, and removes some more pixels
}

/* a function for randomly blanking out pixels
    functions are best used when a repeating operation is needed.
    it allows the programmer to only have to focus on one section of code
    and not needing to copy and paste the same proecedure when needed.
*/
void blank() {
  for (int i = 0; i <= _remove; i++) {
    int rem = random(NUM_LEDS); //randomly flash a brighter green and then blacks it out.
    leds[rem] = 0x010901; FastLED.show(); //quick color flash (rem out if you dont want the flash)
    delay(_speed / 2);
    leds[rem] = 0x000000; FastLED.show(); //black out same point as flashed point (if used)
  }
}
