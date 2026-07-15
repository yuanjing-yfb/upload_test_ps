#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define SDAPIN  16
#define SCLPIN  17

Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
  Serial.begin(9600);
  Wire.setSDA(SDAPIN);
  Wire.setSCL(SCLPIN);
  matrix.begin(0x70, &Wire);
  matrix.clear();
}

void loop() {
  for (int i = 0; i < 4; i++) {
    matrix.writeDigitNum(i, i + 1);
    matrix.writeDisplay();
    delay(500);
  }
  delay(500);
  for (int i = 0; i < 3; i++) {
    matrix.writeDigitNum(0, 8);
    matrix.writeDigitNum(1, 8);
    matrix.writeDigitNum(2, 8);
    matrix.writeDigitNum(3, 8);
    matrix.drawColon(true);
    matrix.writeDisplay();
    delay(500);
    matrix.clear();
    matrix.writeDisplay(); // Blinking effect
    delay(500);
  }
  matrix.clear();
  matrix.writeDisplay(); // Status reset
  delay(1000);
}
