#include <Wire.h>
#include "ws2812.h"

void breathing_Light(uint32_t FlushSpeed);

uint32_t flushSpeed = 20;
PixelStrip strip = PixelStrip(6, 10);
uint32_t colorArray[3] = {0x0000ff, 0x00ff00, 0xff0000};
void setup() {
  Serial.begin(9600);
  while (strip.begin(&Wire));
}
void loop() {
  breathing_Light(flushSpeed);
}
void breathing_Light(uint32_t FlushSpeed) {
  for (int j = 0; j < 3; j++) {
    uint32_t color = colorArray[j];
    for (int i = 0; i < 255; i += 5) {
      strip.fillColor(color);
      strip.setBrightness(i);
      delay(FlushSpeed);
    }
    for (int i = 255; i > 0; i -= 5) {
      strip.fillColor(color);
      strip.setBrightness(i);
      delay(FlushSpeed);
    }
  }
}
