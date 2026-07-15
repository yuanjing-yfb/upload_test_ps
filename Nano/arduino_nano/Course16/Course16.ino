#include <Wire.h>
#include <TCA9534.h>

#define ADDRESS  0x3c //  0x3c---60

byte ret = 0;
byte oldValue = 0;
TCA9534 ioex;

void setup(){
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();
  ioex.attach(Wire);
  ioex.setDeviceAddress(ADDRESS);
  ioex.config(5, TCA9534::Config::IN);
  
  Serial.print("Configures the value of the register-->");
  Serial.println(ioex.config(), HEX);
  Serial.print("Output the value of the register-->");
  Serial.println(ioex.output(), HEX);
}

void loop(){
  byte ret = ioex.input(5);
  if( !ret && oldValue != ret ) {
    Serial.println("Magnet near");
    oldValue = ret;
  } else if(ret && oldValue != ret) {
    Serial.println("Magnet departure");
    oldValue = ret;
  }
  delay(500);
}
