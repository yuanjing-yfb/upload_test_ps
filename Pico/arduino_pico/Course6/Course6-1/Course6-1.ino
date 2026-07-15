#include <Wire.h>
#include <TCA9534.h>

#define ADDRESS  0x3c
#define SDAPIN    16
#define SCLPIN    17

int position = 0;
int oldPosition = 0;
int valueA = 0;
int valueB = 0;
int oldValueA = 0;

TCA9534 ioex;

void setup(){
  Serial.begin(9600);
  Wire.setSDA(SDAPIN);
  Wire.setSCL(SCLPIN);
  Wire.begin();
  ioex.attach(Wire);
  ioex.setDeviceAddress(ADDRESS);
  ioex.config(6, TCA9534::Config::IN);
  ioex.config(7, TCA9534::Config::IN);

  Serial.println("[INFO] Set up done!");
}

void loop(){
  read_quadrature();
  if( oldPosition != position) {
    Serial.print("Position: ");
    Serial.println(position, DEC);
    oldPosition = position;
  }
}


void read_quadrature(){
  valueA = ioex.input(6);
  valueB = ioex.input(7);
  if (valueA != oldValueA) {
    oldValueA = valueA;
    if (valueA == 0) {
      if (valueB == 0) {
        position--;
        if(position < 0)  position = 0;
      }
    } else {
      if (valueB == 0) {
        position++;
        if(position > 1023)  position = 1023;
      }
    }
  }
}
