#include "FastLED.h"
#include "pitches.h"

#define NUM_LEDS 6
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
// notes in the melody:
int melody[] = {
  NOTE_G5, NOTE_A5, NOTE_F5, NOTE_F3, NOTE_C5
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 4, 4, 1
};
// constants won't change. They're used here to
// set pin numbers:
const int relayPin = 1;     // the number of the pushbutton pin
const int buttonPin = 2;     // the number of the pushbutton pin
// neopixel pin in #define
// const int  =  11;      // the number of the relay pin
const int rPin =  13;      // the number of the LED pin
const int bPin =  12;      // the number of the LED pin
// const int sPin =  11;      // the number of the LED pin
const int aOut =  10;      // the number of the LED pin


// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
void tones() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 5; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(aOut, melody[thisNote], noteDuration);

    leds[thisNote] = CRGB::Green;
    FastLED.show();

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(5);

    leds[thisNote] = CRGB::Black;
    FastLED.show();
  }

}
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // initialize the LED pin as an output:
  pinMode(relayPin, OUTPUT);
  pinMode(rPin, OUTPUT);
//  pinMode(sPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  leds[5] = CRGB::White;
  FastLED.show();

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  //#if defined (__AVR_ATtiny85__)
  //  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  // End of trinket special code
}

void loop() {
  leds[5] = CRGB::DarkBlue;
  FastLED.show();
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn relay on:
    digitalWrite(relayPin, HIGH);
    digitalWrite(bPin, HIGH);
    digitalWrite(rPin, LOW);
    leds[5] = CRGB::Green;
    FastLED.show();
    tones();
    delay(2000);
  } else {
    // turn relay off:
    digitalWrite(rPin, HIGH);
    digitalWrite(bPin, LOW);
    digitalWrite(relayPin, LOW);
  }
}

