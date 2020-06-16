//this clock uses 4 , 15 pixel sements, pin 6 is data, RTC is DS3231 connected to SCA/SCL
// clock is set with exact date ant time of last programme compile as reference, will be off by ~15 sec compile time +upload time

#include <Time.h>
#include <TimeLib.h>
#include <FastLED.h>
#define NUM_LEDS 60 // how many total pixels are going to be used here
#define SEG 4 //sets up weither each segment is in parts
#define PPS 15 //Pixels Per Segments
#define DATA_PIN 6
#include <Wire.h>
#include "RTClib.h"


RTC_DS3231 rtc;
CRGB pixel[NUM_LEDS]; // sets up array of usable pixels in the programme

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(pixel, NUM_LEDS); // initalize the available matrix using information in the #define statements
  for (int blank = 0; blank < NUM_LEDS; blank++) { // set all available pixels off,
    pixel[blank] = 0x000000;
    FastLED.show();
  }
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() {

  DateTime now = rtc.now();
  int secPix = now.second();
  int minPix = now.minute();
  int hrPix = now.hour() * 5;
  if (now.hour() >= 12) // adds correction for 24 hour clock time
  {
    hrPix = hrPix - 60;
  }
  //  add correction field to update time() every hour
  // sets second pixels with corrections
  if (second() != 59) // removes last second pixel, if its not :59 anymore
  {
    pixel[59] = 0x000000;
  }
  pixel[secPix - 1] = 0x000000; pixel[secPix] = 0x111111;// set sec pixel on

  // sets minute pixels
  pixel[minPix - 1] = 0x000000; pixel[minPix] = 0x000055; // set min pixel on and correct
  if (pixel[minPix - 1] == pixel[secPix])  {
    (pixel[minPix - 1]) = 0x111111;
  }


  // set hour pixels
  pixel[hrPix - 1] = 0x000000; pixel[hrPix] = 0x550000; // set hour pixel on and correct

  //make some corrections
  if (now.second() && now.minute() == 0) {
    pixel[hrPix - 5] = 0x000000;
  }
  if (pixel[hrPix - 1] == pixel[secPix]) {
    (pixel[hrPix - 1]) = 0x111111;
  }
    if (pixel[hrPix - 1] == pixel[minPix]) {
    (pixel[minPix - 1]) = 0x000055;
  }
  if (pixel[hrPix] == pixel[secPix])  {
    (pixel[hrPix]) = 0x551111;
  }
  if (pixel[hrPix] == pixel[minPix])  { //min & hour overlap
    pixel[hrPix] = 0x550055;
  }
  if (pixel[minPix - 1] == pixel[hrPix])  {
    (pixel[minPix - 1]) = 0x110000;
  }
  if (pixel[minPix] == pixel[secPix]) {
    (pixel[minPix]) = 0x111155;
  }
  //corrections (overlapping, pixel clearning...)






  FastLED.show(); // show me the money (current time now)
}
