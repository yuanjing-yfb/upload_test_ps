#include <Wire.h>
#include <TCA9534.h>
#define ADDRESS  0x3c    
TCA9534 ioex;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  ioex.attach(Wire);
  ioex.setDeviceAddress(ADDRESS);
  ioex.config(4, TCA9534::Config::OUT);
}

void loop() {
  Serial.println("Vibration active LOW");
  ioex.output(4, TCA9534::Level::L);
  delay(500);
  Serial.println("Vibration active HIGH");
  ioex.output(4, TCA9534::Level::H);
  delay(500);
}
