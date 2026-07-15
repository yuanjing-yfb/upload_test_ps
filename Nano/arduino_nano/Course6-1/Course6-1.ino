#include <Wire.h>
#include <TCA9534.h>

#define ADDRESS  0x3c

int position = 0;
int oldPosition = 0;
int valueA = 0;
int valueB = 0;
int oldValueA = 0;

/* Defining an iic extension to control GPIOs */
TCA9534 ioex;

#define Buzzer_pin 6

void setup(){
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  Wire.begin();
  ioex.attach(Wire);
  ioex.setDeviceAddress(ADDRESS);
  /* Setting pins 6 and 7 of the ic of the iic extended GPIO to input state */
  ioex.config(6, TCA9534::Config::IN);
  ioex.config(7, TCA9534::Config::IN);

  Serial.println("[INFO] Set up done!");
}

void loop(){
  read_quadrature();
  if( oldPosition != position) {
    oldPosition = position;
    /* Invert PWM */
    analogWrite(Buzzer_pin, 255-position);
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
