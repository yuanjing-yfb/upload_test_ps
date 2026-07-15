#include <HCSR04.h>
#include "Adafruit_LiquidCrystal.h"
#include <Wire.h>
#define SDAPIN  16
#define SCLPIN  17

const byte triggerPin = 3;
const byte echoPin = 9;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

Adafruit_LiquidCrystal lcd(1);

void setup () {
  Serial.begin(9600);
  Wire.setSDA(SDAPIN);
  Wire.setSCL(SCLPIN);
  Wire.begin();
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
