#include <Wire.h>
#include "ws2812.h"

void Running_light(uint32_t FlushSpeed);

uint32_t flushSpeed = 200;
PixelStrip strip = PixelStrip(6, 10);

void setup() {
  Serial.begin(9600);
  while (strip.begin(&Wire));
  Running_light(flushSpeed);
}

void loop() {
  /* Flow lamp experiment */
  Running_light(flushSpeed);
  delay(1000);
}

void Running_light(uint32_t FlushSpeed) {
  // Red
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
  // And then one by one, they go out.
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
  // Green
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
  // And then one by one, they go out.
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
  // Blue
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
  // And then one by one, they go out.
  for (uint16_t i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    delay(FlushSpeed);
  } delay(FlushSpeed);
}
