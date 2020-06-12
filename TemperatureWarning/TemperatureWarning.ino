#include <Adafruit_CircuitPlayground.h>

float tempC, tempF;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(10);                       // wait for a second
  tempC = CircuitPlayground.temperature();
  tempF = CircuitPlayground.temperatureF();

  Serial.print("tempC: ");
  Serial.print(tempC);
  Serial.print("  tempF: ");
  Serial.println(tempF);
  if (tempF < 95) {
    for (int x = 0; x <= 9; x++) {
      CircuitPlayground.setPixelColor(x, 0,   tempC,   0);
    }
    delay(1000);
  }
  else {
    for (int x = 0; x <= 9; x++) {
      CircuitPlayground.setPixelColor(x, 255,   0,   0);
    }
    CircuitPlayground.playTone(1000, 50);
    delay(15000);
  }
}
