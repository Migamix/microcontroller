//a little programme to detect motion on a 3d printer
//and light up the build plate for a octoprint based PI camera
//while providing some status information via the CPX sensors
// colors chosen and brighness are ideal for me
//as the camera wont have to AWB much and it has a cool blacklight effect

#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;
float Xold, Yold, Zold;
int count;
void setup() {
  Serial.begin(9600); // sending data to serial, for future project
  CircuitPlayground.begin(); //CP init
  CircuitPlayground.setBrightness(30);
}

//ACTIVE lights when detecting motion
void lightsOn() {
  CircuitPlayground.setBrightness(100);
  for (int x = 0; x < 10; x++) {
    CircuitPlayground.setPixelColor(x, 0xFFFFFF);
  }
  delay(1000);
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
  //reset counter to zero when this function is called because of movement
  count = 0;
}

//IDLE lights when not detecting motion
void lightsSB() {
  CircuitPlayground.setBrightness(50);
  for (int x = 0; x < 9; x++) {
    CircuitPlayground.setPixelColor(x, 0x0000FF);
  }
  CircuitPlayground.setPixelColor(9, CircuitPlayground.temperature(), 0, 0); //sets last pixel to red based on TempC
}


void loop() {
  // dumps old value into variable to compare with later before getting a new value
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
  Serial.print("  snd ");
  Serial.print(CircuitPlayground.mic.soundPressureLevel(10));
  Serial.println();
  
  //compare difference of each direction. 
  // best to include a floating number if adjusting
  if ( Xold >= X + .99 || Xold <= X - .99) {
    lightsOn();
  }
  if ( Yold >= Y + .99 || Yold <= Y - .99) {
    lightsOn();
  }
  if ( Zold >= Z + 1.99 || Zold <= Z - 1.99) {
    lightsOn();
  }
  //turns lights on and keeps them on as long as its above given temp C
  if (CircuitPlayground.temperature() >= 60) { 
    lightsOn();
    count = 0;
  }

  // will loop a given number of times every 100 ms given by the delay below
  // then jump to the standby lights after that point, begins the loop again
  if (count == 300) {
    lightsSB();
    count = 0;
  }
  count++;
  delay(100);
}
