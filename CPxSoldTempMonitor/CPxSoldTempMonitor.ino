#include <Adafruit_CircuitPlayground.h>

float tempC, tempF;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { // use modified blink sketch for polling identifyer
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(10);                       // wait 10ms
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(10);                       // wait 10ms
  tempC = CircuitPlayground.temperature();
  tempF = CircuitPlayground.temperatureF();

  Serial.print("tempC: ");
  Serial.print(tempC);
  Serial.print("  tempF: ");
  Serial.println(tempF);
  if (tempC < 33) {
    for (int x = 0; x <= 9; x++) {
      CircuitPlayground.setPixelColor(x, 0,   tempC,   0); //(pixel#, RRR, GGG, BBB)
    }
    delay(1000);
  }
  else {
    for (int x = 0; x <= 9; x++) {
      CircuitPlayground.setPixelColor(x, tempF,   0,   0);
    }
    CircuitPlayground.playTone(1000, 50);
    delay(15000);
  }
}
