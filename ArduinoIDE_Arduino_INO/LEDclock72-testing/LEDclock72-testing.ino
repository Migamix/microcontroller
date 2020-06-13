/*this clock uses 4 15 (5050)pixel quater sements, and a single 12 pixel ring tied to the output of the first set (always second)
  pin 6 is data to neopixels,
  RTC is DS3231 connected to SDA(A4)/SCL(A5) and its power is from the microcontroller
  the trinket pro was used for this project https://www.adafruit.com/products/2000
  for good information on the DS3231 clock module, see https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/wiring-and-test
  ***additional notes are at the bottom of code***
*/
//#include <Time.h>
#include <util/usa_dst.h> //daylight saving setup
//#include <TimeLib.h>
#include <FastLED.h> // neopixel library in use
#include <Wire.h> //to use the SDA/SCL RTC module
#include <RTClib.h> // RTC module commands

#define DATA_PIN 6 // the single pin where the neopixel gets their orders from 
//(#6 is most comonly used in most code for neopixels, but you can change this if you need)
int buttonInc = 9; // increase time hour()
int buttonDec = 10; // decrease time hour()
// press both buttons at same time to put clock in time setting mode
#define NUM_LEDS 72 // how many TOTAL pixels are going to be used here (2 rings of 60px and 12px)
//pin A5 used on trinket pro for RTC SCL (leave as comment, using for building instructions)
//pin A4 used on trinket pro for RTC SDA (leave as comment, using for building instructions)

//sets up colours to be used on clock. all HEX based, think 0xRRGGBB
// insert setColor // used to indicate clock is in setup mode, to change time with buttons.
#define bgColor 0x000000 //its best to keep this as all off for lowest power consumption, or using dim values like white=0x010101
#define hourColor 0x005500
#define minColor 0x000055
#define secColor 0x550000
#define tixColor 0x111111

RTC_DS3231 rtc;
CRGB pixel[NUM_LEDS]; // sets up array of usable pixels in the programme from the FastLED lib.
DateTime theTime; // Holds current clock time

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(pixel, NUM_LEDS); // initalize the available neopixel matrix using information in the #define statements
  for (int blank = 0; blank < NUM_LEDS; blank++) { // set all available pixels off,
    pixel[blank] = bgColor;
    FastLED.show();
  }
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  delay(1000); // wait for console opening
  if (! rtc.begin()) {
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
    theTime = rtc.now();
    // DST? If we're in it, let's subtract an hour from the RTC time to keep our DST calculation correct. This gives us
    // Standard Time which our DST check will add an hour back to if we're in DST.
    if (OBSERVE_DST == 1) {
      if (checkDst() == true) { // check whether we're in DST right now. If we are, subtract an hour.
        theTime = theTime.unixtime() - 3600;
      }
    }
    RTC.adjust(theTime);
  }
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  pinMode(buttonInc, INPUT);
  pinMode(buttonDec, INPUT);
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  /*the following is used to calabrate the clock
    remeber* final compile and upload of correct time will be needed to fix the time when testing is done.
    or it will keep restting the to its last saved time.
    to make sure the time is recieved only from the RTC, comment out the adjusting code, and reupload,
    once the clock is set, and a battery is installed, you should not need to do this again.
    you will even have the ability to adjust the code once the RTC is set*/

  // This line sets the RTC with an explicit date & time, for example to set
  // Feb 21, 2018 at 21:07:00 you would call: " rtc.adjust(DateTime(2018, 02, 21, 21, 07, 00)); "
  //rtc.adjust(DateTime(2018, 02, 21, 21, 07, 00));  // rtc.adjust(DateTime(YYYY, M, D, HH, M, S));"
}
int buttonIncState = 0;
int buttonDecState = 0;
void loop() {
  DateTime now = rtc.now();
  int secPix = now.second();
  int minPix = now.minute();
  int hrPix = now.hour() + 60; // pushes the hours out of the min/sec ring, and onto its own 12 pixel ring
  // to detect button press and force adjust clock
  buttonIncState = digitalRead(buttonInc);
  if (buttonIncState == HIGH) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 1, now.minute(), now.second()));
    delay(500);
  }
  buttonDecState = digitalRead(buttonDec);
  if (buttonDecState == HIGH) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() - 1, now.minute(), now.second()));
    delay(500); pixel[hrPix + 2] = bgColor;
  }
  int tix = 0;
  for (; tix > 55; tix = tix + 5) {
    pixel[tix] = tixColor;
  }
  // FastLED.show();
  /*pixel[0] =  tixColor; pixel[5] =  tixColor; pixel[10] =  tixColor; pixel[15] =  tixColor;
    pixel[20] =  tixColor; pixel[25] =  tixColor; pixel[30] =  tixColor;
    pixel[35] =  tixColor; pixel[40] =  tixColor; pixel[45] =  tixColor; pixel[50] =  tixColor; pixel[55] =  tixColor;*/

  // set hour pixels -----------------------------------------------------------
  if (now.hour() >= 12) { // adds correction for 24 hour clock time. (24 hour time kept for future projects and easy to correct for)
    hrPix = hrPix - 12;
  }
  if (now.hour() != 11 || now.hour() != 23) { // removes last hour pixel, if its not 11 anymore
    pixel[60 + 11] = bgColor;
  }
  pixel[hrPix - 1] = bgColor; // set hour-1 pixel to bgcolor, this causes issues with 60pixel ring if hours are not set first
  pixel[hrPix] = hourColor; // set hour pixel on

  // set Second pixels -----------------------------------------------------------
  if (now.second() != 59) { // removes last second pixel, if its not :59 anymore
    pixel[59] = bgColor; // sets that pesky #59 to the background color since secPix -1 wont cut it if sec is now 0
  }
  pixel[secPix - 1] = bgColor; // set sec-1 pixel to bgcolor
  pixel[secPix] = secColor; // set sec pixel on

  // set minute pixels -----------------------------------------------------------
  pixel[minPix - 1] = bgColor; //min-1 pixel to bgcolor
  pixel[minPix] = minColor; // set min pixel on
  if (pixel[minPix - 1] == pixel[secPix])  {
    (pixel[minPix - 1]) = secColor; // sets the "second" pixel that is only 1 pixel behind the minute pixel on
  }
  if (pixel[minPix] == pixel[secPix]) {
    (pixel[minPix]) = (minColor + secColor);
  }
  FastLED.show(); // show me the money (current time with correction now displayed)

}

/*
  to be sure all pixels show, with corrections, and prevention of overlap issues, make sure to use this order to display
  hour()
  second()
  min()

*/

/*
    Function to figure out if we're in Daylight Saving Time, then adding an hour if we are in DST.
    adapted from nseidle
    https://github.com/nseidle/Daylight_Savings_Time_Example/blob/master/Daylight_Savings_Time_Example.ino

    This algorithm is programmed to observe Daylight Saving Time in the United States, where as of the time of writing  DST is observed
    between the second Sunday in March and the first Sunday in November. The rules for DST vary by country and territory.
    https://en.wikipedia.org/wiki/Daylight_saving_time_by_country

    If you're in a territory which observes DST differently this code will need to be modified. If you're lucky enough to not observe DST
    then much of this code can be commented out!

    This method doesn't check whether its 2am or not when the time change officially occurs. This could be more accurate at the expense of being more complicated.

*/
DateTime calculateTime() {

  DateTime RTCTime = rtc.now();

  if (OBSERVE_DST == 1) {
    if (checkDst() == true) {
      RTCTime = RTCTime.unixtime() + 3600;  // add 1 hour or 3600 seconds to the time
    }
  }

  /*
    Serial.print(RTCTime.year(), DEC);
    Serial.print('/');
    Serial.print(RTCTime.month(), DEC);
    Serial.print('/');
    Serial.print(RTCTime.day(), DEC);
    Serial.print(' ');
    Serial.print(RTCTime.hour(), DEC);
    Serial.print(':');
    Serial.print(RTCTime.minute(), DEC);
    Serial.print(':');
    Serial.print(RTCTime.second(), DEC);
    Serial.println();
  */

  return RTCTime;
}

boolean checkDst() {

  DateTime RTCTime = rtc.now();

  //Get the day of the week. 0 = Sunday, 6 = Saturday
  int previousSunday = RTCTime.day() - RTCTime.dayOfTheWeek();

  boolean dst = false; //Assume we're not in DST
  if (RTCTime.month() > 3 && RTCTime.month() < 11) dst = true; //DST is happening!

  //In March, we are DST if our previous Sunday was on or after the 8th.
  if (RTCTime.month() == 3)
  {
    if (previousSunday >= 8) dst = true;
  }
  //In November we must be before the first Sunday to be dst.
  //That means the previous Sunday must be before the 1st.
  if (RTCTime.month() == 11)
  {
    if (previousSunday <= 0) dst = true;
  }

  return dst;

}
