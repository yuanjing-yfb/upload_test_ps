#include "LED.hpp"
/* Define the speed at which the running lights flow--ms */
uint32_t flushSpeed = 20;
PixelStrip strip = PixelStrip(6, 10);
uint32_t colorArray[3] = {0x0000ff, 0x00ff00, 0xff0000};

void setup() {
  Wire.begin();
  strip.begin();
}

void loop() {
  /* Breathing light experiment */
  breathing_Light(flushSpeed);
}

void breathing_Light(uint32_t FlushSpeed) {
  for(int j = 0; j <3; j++) {
    uint32_t color = colorArray[j];
    for(int i = 0; i < 255; i+=5) {
      strip.fillColor(color);
      strip.setBrightness(i);
      delay(FlushSpeed);
    }
    for(int i = 255; i > 0; i-=5) {
      strip.fillColor(color);
      strip.setBrightness(i);
      delay(FlushSpeed);
    }
  }
}
