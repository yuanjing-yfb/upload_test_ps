#include <Wire.h>
#include "ws2812.h"

#define flame_pin 7
PixelStrip strip = PixelStrip(6, 20);

static byte oldState = 0;
static byte flameState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(flame_pin, INPUT);
  strip.begin(&Wire);
  strip.clear();
}

void loop() {
  flameState = digitalRead(flame_pin);
  if (!flameState && oldState != flameState) {
    Serial.println("[-] Flame Detect!");
    oldState = flameState;
    strip.setPixelColor(1, strip.Color(255, 0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0, 0));
    delay(1000);
  } else if (flameState && oldState != flameState) {
    Serial.println("[-] Flame not Detect!");
    oldState = flameState;
    strip.setPixelColor(2, strip.Color(0, 255, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0, 0));
    delay(1000);
  }
}
