#include <Wire.h>
#include "ws2812.h"

/* Define the speed at which the running lights flow--ms */
uint32_t flushSpeed = 200;
PixelStrip strip = PixelStrip(6, 10);
void setup() {
  Serial.begin(9600);
  strip.begin(&Wire);
  strip.clear();
}
void loop() {
  traffic_Light();
  strip.clear();
  delay(1000);
}
void traffic_Light() {
  /* Green light on for 5 seconds  */
  strip.setPixelColor(0, strip.Color(0, 255, 0, 0));
  strip.setPixelColor(3, strip.Color(0, 255, 0, 0));
  delay(5000);
  strip.clear();
  /* Yellow light on for 3 seconds */
  strip.setPixelColor(1, strip.Color(255, 255, 0, 0));
  strip.setPixelColor(4, strip.Color(255, 255, 0, 0));
  delay(3000);
  strip.clear();
  /* Red light on for 5 seconds */
  strip.setPixelColor(2, strip.Color(255, 0, 0, 0));
  strip.setPixelColor(5, strip.Color(255, 0, 0, 0));
  delay(5000);
  strip.clear();
}
