#include <HCSR04.h>
#include "Adafruit_LiquidCrystal.h"

const byte triggerPin = A1;
const byte echoPin = 5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

Adafruit_LiquidCrystal lcd(1);

void setup () {
  Serial.begin(9600);
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
}

void loop () {
  float distance = distanceSensor.measureDistanceCm();
  if (distance > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance(Cm):");
    lcd.setCursor(0, 1);
    lcd.print(distance);
  }
  delay(500);
}
