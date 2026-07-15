#include "ws2812.h"
#include <Wire.h>

PixelStrip strip = PixelStrip(64, 10);

void setup()
{
  Serial.begin(9600);

  Wire.begin();
  strip.begin();

  RGB_matrix_flicker(strip);
  Serial.println("setup done!");
}

void loop()
{
  RGB_matrix_flicker(strip);
  delay(1000);
}

void RGB_matrix_flicker(const PixelStrip &strip)
{
  strip.fill(255, 0, 0);
  delay(500);
  strip.fill(0, 255, 0);
  delay(500);
  strip.fill(0, 0, 255);
  delay(500);
  strip.fill(0, 255, 255);
  delay(500);
  strip.fill(255, 255, 0);
  delay(500);
  strip.fill(255, 0, 255);
  delay(500);
  strip.fill(255, 255, 255);
  delay(500);
  strip.clear();
}
