#include "LED.hpp"
#include <SPI.h>

#define SS_PIN 10
#define Key_Channel  0x03
#define Buzzer_pin 6

// Lamp strip initialization
PixelStrip strip = PixelStrip(6, 20);
uint32_t flushSpeed = 200;

// Key voltage threshold
static byte key_val[] = {190, 205, 220, 235}; 
bool keyOneFlag = true;

void setup() {
  Serial.begin(115200);

  // SPI initialization
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  pinMode(SS_PIN, OUTPUT);
  // digitalWrite(SS_PIN, HIGH);

  // Lamp strip initialization
  strip.begin();
  strip.clear();

  // Buzzer initialization
  pinMode(Buzzer_pin, OUTPUT);
  digitalWrite(Buzzer_pin, LOW);

  Serial.println("[INFO] Setup done");
}

void loop() {
  byte val = readChannel(Key_Channel);
  delay(100);
  byte keyNum = getKeyNum(val);

  if (keyNum != 255) {
    switch (keyNum) {
      case 0: Serial.println("KEY 1 pressed"); keyOneFunction(); break;
      case 1: Serial.println("KEY 2 pressed"); keyTwoFunction(); break;
      case 2: Serial.println("KEY 3 pressed"); keyThreeFunction(); break;
      case 3: Serial.println("KEY 4 pressed"); keyFourFunction(); break;
    }
    delay(300);
  }
}

// SPI Read Function
byte readChannel(byte channel) {
  digitalWrite(SS_PIN, LOW);
  byte value = SPI.transfer(channel);
  digitalWrite(SS_PIN, HIGH);
  return value;
}

// SPI reading function
byte getKeyNum(byte val) {
  for (int i = 0; i < 4; i++) {
    if (val <= key_val[i]) return i;
  }
  return 255;
}

// Lamp control function 1: Full on/clear switch+buzzer
void keyOneFunction() {
  strip.clear();
  if (keyOneFlag) {
    keyOneFlag = false;
    strip.fill(255, 255, 255);
    beep(500);
  } else {
    keyOneFlag = true;
    strip.clear();
    beep(500);
  }
}

// Lamp control function 2: red, green, yellow signal light logic+buzzer
void keyTwoFunction() {
  strip.clear();

  // red led
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.setPixelColor(3, strip.Color(255, 0, 0));
  beep(1000);
  strip.clear();
  delay(1000);

  // green led
  strip.setPixelColor(1, strip.Color(0, 255, 0));
  strip.setPixelColor(4, strip.Color(0, 255, 0));
  beep(500);
  delay(500);
  strip.clear();
  delay(1000);

  // yellow led
  strip.setPixelColor(2, strip.Color(255, 255, 0));
  strip.setPixelColor(5, strip.Color(255, 255, 0));
  beep(200);
  delay(300);
  beep(500);
  strip.clear();
}

// Lamp control function 3: Random color flashing
void keyThreeFunction() {
  for (int i = 0; i < 6; i++) {
    uint32_t color = random(0, 16777215);
    strip.setPixelColor(i, color);
    delay(500);
    strip.clear();
  }
}

// Lamp control function 4: Gradient color scrolling
void keyFourFunction() {
  static unsigned long color = 0x000000;
  for (int j = 0; j < 5; j++) {
    for (int k = 0; k < 6; k++) {
      for (int l = 0; l < 100; l++) {
        strip.setPixelColor(k, color++);
      }
    }
  }
}

// Buzzer sound packaging
void beep(int ms) {
  digitalWrite(Buzzer_pin, LOW);
  delay(ms);
  digitalWrite(Buzzer_pin, HIGH);
}
