#include <Servo.h>

Servo myservo;
int pos = 0;

void setup() {
  //Explicitly specify the actual supported pulse width range of the servo (unit: microseconds)
  //For example, SG90: 1000~2000 microseconds

  myservo.attach(8, 500, 2500);  // pin 8, min pulse 500us, max pulse 2500us
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
}
