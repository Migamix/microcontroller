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
int idle = 60;
int moving = 60;
int i = 0; int s=0;
RTC_DS3231 rtc;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Booting clock"); delay(100);
  lcd.setBacklight(OFF);
  delay(500); // wait for console opening
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    lcd.print("Couldn't find RTC");
  }


  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    lcd.print("RTC lost power..."); delay(2000);
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
//uint8_t i = 0;
   // setTimeYear();

    
    delay(1000); lcd.clear();
    lcd.print("Month "); lcd.print(setMonth); delay(1000);
    delay(1000); lcd.clear();
    lcd.print("Day "); lcd.print(setDay); delay(1000);
    delay(1000); lcd.clear();
    lcd.print("Hour "); lcd.print(setHour); delay(1000);
    delay(1000); lcd.clear();
    lcd.print("Min "); lcd.print(setMin); delay(1000);
    delay(1000); lcd.clear();
    lcd.print("Sec "); lcd.print(setSec); delay(1000);
    delay(1000); lcd.clear();

    int push;
    push = lcd.readButtons();

    if (push & BUTTON_UP) {
      mMode++;
      setup();
    }
  }
}
void lcdFlash() {
  lcd.setBacklight(OFF); delay(50);
  lcd.setBacklight(RED); delay(50);
  lcd.setBacklight(BLUE); delay(50);
  lcd.setBacklight(GREEN); delay(50);
  lcd.setBacklight(YELLOW); delay(50);
  lcd.setBacklight(TEAL); delay(50);
  lcd.setBacklight(VIOLET); delay(50);
  lcd.setBacklight(WHITE); delay(500);
  lcd.clear();
}

void setTime() { // enter time setting function
  DateTime now = rtc.now();
  int mMode = 0;
  int setMonth = now.month();
  int setDay = now.day();
  int setHour = now.hour();
  int setMin = now.minute();
  int setSec = now.second();
  while (mMode == 0) {
    lcd.clear(); lcd.print("entering setup");

    lcdFlash();





void pushTheButton() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    lcd.setCursor(0, 0);
    if (buttons & BUTTON_SELECT) {
      lcd.setBacklight(WHITE);
      setTime();
    }
    if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(WHITE);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_UP) {
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.setBacklight(BLUE);
    }
    if (buttons & BUTTON_LEFT && buttons & BUTTON_UP) {
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT && buttons & BUTTON_RIGHT) {
      lcd.setBacklight(VIOLET);
    }
    if (buttons & BUTTON_RIGHT && buttons & BUTTON_UP) {
      lcd.setBacklight(TEAL);
    }
    delay(300);
  }
}

void setButton() {
  while (s > 0){
  uint8_t buttons = lcd.readButtons();
    if (buttons) {
    if (buttons & BUTTON_SELECT) {
      setup();
    }
    if (buttons & BUTTON_DOWN) {
    i--;
    }
    if (buttons & BUTTON_LEFT) {
    s--;
    }
    if (buttons & BUTTON_UP) {
    i++;
    }
    if (buttons & BUTTON_RIGHT) {
    s++;
    }
    delay(50);
    
  }
}
void setTimeYear() {
  DateTime now = rtc.now();
  int setYear = now.year();
    lcd.print("Year ");
    lcd.setCursor(0, 1); lcd.print(setYear);
    setbutton
}


void loop() {
  DateTime now = rtc.now();

  idle = now.second();
  //attempt to remove flicker
  if (idle == moving) {
    pushTheButton();
  }
  /*
    //set backlight based on time of day
      if (now.hour() <= 6 || now.hour() >= 19 ) {
        lcd.setBacklight(GREEN);
      }
      else {
        lcd.setBacklight(WHITE);
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
  moving = now.second();
  // add the day of the week after the time
  lcd.print("    ");
  static const char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  // set position and display date in ISO format. YYYY/MM/DD
  lcd.setCursor(6, 1); //(6, 1)= going to the sixth position on next line of the display...6 over, 1 down
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

  pushTheButton();
}
