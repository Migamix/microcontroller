//a little programme to detect motion on a 3d printer
//and light up the build plate for a octoprint based PI camera 
//while providing some status information via the CPX sensors

#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;
float Xold, Yold, Zold;
int count;
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(30);
  lightsSB();
}

void lightsOn() {
  CircuitPlayground.setBrightness(100);
  for (int x = 0; x < 10; x++) {
    CircuitPlayground.setPixelColor(x, 0xffffff);
  }
  delay(1000);
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
  count = 0;
}

void lightsSB() {
  CircuitPlayground.setBrightness(50);
  for (int x = 0; x < 9; x++) {
    CircuitPlayground.setPixelColor(x, 0x0000FF);
  }
  CircuitPlayground.setPixelColor(9, CircuitPlayground.temperature(), 0, 0);
}

void loop() {
  Xold = X;
  Yold = Y;
  Zold = Z;
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

//debugging output to serial
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.print(Z);
  Serial.print("  count: ");
  Serial.print(count);
  Serial.print("  temp ");
  Serial.print(CircuitPlayground.temperature());

//Serial.print("  snd ");
//Serial.print(CircuitPlayground.mic.soundPressureLevel(10));

  Serial.println();

  if ( Xold >= X + .99 || Xold <= X - .99) {
    lightsOn();
  }
  if ( Yold >= Y + .99 || Yold <= Y - .99) {
    lightsOn();
  }
  if ( Zold >= Z + 1.99 || Zold <= Z - 1.99) {
    lightsOn();
  }
  if (CircuitPlayground.temperature() >= 60) {
    lightsOn();
    count = 0;
  }

  if (count == 500) { 
    //    CircuitPlayground.clearPixels();
    lightsSB();
    count = 0;
  }
  count++;
  delay(100); // this number allows for checks for changes in a ballanced way, not too fast/slow
}
