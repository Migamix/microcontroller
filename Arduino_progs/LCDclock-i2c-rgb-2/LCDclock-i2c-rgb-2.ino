// RTClib - Version: 1.2.0


/*********************

  code for the Adafruit RGB Character LCD Shield and Library

  This code displays time on the shield, and also reads the buttons on the keypad.
  When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <RTClib.h> // RTC module commands

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define OFF 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define VIOLET 0x5
#define TEAL 0x6
#define WHITE 0x7

RTC_DS3231 rtc;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  delay(500); // wait for console opening
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  lcd.setBacklight(OFF);
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis();
  Serial.print("starting ...");
  time = millis() - time;
  Serial.print(" Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);

  /*the following is used to calabrate the clock
    remeber* final compile and upload of correct time will be needed to fix the time when testing is done.
    or it will keep restting the to its last saved time.
    to make sure the time is recieved only from the RTC, comment out the adjusting code, and reupload,
    once the clock is set, and a battery is installed, you should not need to do this again.
    you will even have the ability to adjust the code once the RTC is set*/

  // This line sets the RTC with an explicit date & time, for example to set
  // Feb 21, 2018 at 21:07:00 you would call: " rtc.adjust(DateTime(2018, 02, 21, 21, 07, 00)); "
  // rtc.adjust(DateTime(2018, 02, 22, 00, 9, 45));  // rtc.adjust(DateTime(YYYY, M, D, HH, M, S));"
}


uint8_t i = 0;
void loop() {
  DateTime now = rtc.now();
  //set backlight based on time of day
/*  if (now.hour() <= 6 || now.hour() >= 19 ) {
    lcd.setBacklight(GREEN);
  }
  else {
    lcd.setBacklight(OFF);
  }
*/
  //display time on first row
  lcd.setCursor(0, 0);
  if (now.hour() < 10) { //add '0' padding to time's with a single dig.
    lcd.print('0');
  };
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) {
    lcd.print('0');
  };
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second() < 10) {
    lcd.print('0');
  };
  lcd.print(now.second());

  // add the day of the week after the time
  lcd.print("    ");
  static const char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

  // set position and display daate in ISO format. YYYY/MM/DD
  lcd.setCursor(6, 1); //(2, 1)= going to the second position on next line of the display...2 over, 1 down
  lcd.print(now.year(), DEC);
  lcd.print('/');
  if (now.month() < 10) {
    lcd.print('0');
  }
  lcd.print(now.month(), DEC);
  lcd.print('/');
  if (now.day() < 10) {
    lcd.print('0');
  }
  lcd.print(now.day(), DEC);


  uint8_t buttons = lcd.readButtons();
  if (buttons) {
//    lcd.clear();
    lcd.setCursor(0, 0);
    if (buttons & BUTTON_SELECT) {
//      lcd.clear();
      lcd.setBacklight(OFF);
    }
    if (buttons & BUTTON_DOWN) {
// /     lcd.clear();
      lcd.setBacklight(WHITE);
    }
    if (buttons & BUTTON_LEFT) {
// /     lcd.clear();
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_UP) {
///      lcd.clear();
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
///      lcd.clear();
      lcd.setBacklight(BLUE);
    }
     if (buttons & BUTTON_LEFT && buttons & BUTTON_UP) {
//  /    lcd.clear();
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT && buttons & BUTTON_RIGHT) {
///      lcd.clear();
      lcd.setBacklight(VIOLET);
    }
     if (buttons & BUTTON_RIGHT && buttons & BUTTON_UP) {
///    lcd.clear();
      lcd.setBacklight(TEAL);
    }
    
    
    delay(200);
  }
}
