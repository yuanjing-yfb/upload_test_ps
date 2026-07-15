#include "ws2812.h"
#include <Wire.h>

PixelStrip strip = PixelStrip(64, 10);

void setup()
{
  Serial.begin(9600);
  // while (!Serial);
  
  while(strip.begin(&Wire));
  RGB_matrix_flicker(strip);
}

void loop()
{
  RGB_matrix_flicker(strip);
}

void RGB_matrix_flicker(PixelStrip &strip)
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
