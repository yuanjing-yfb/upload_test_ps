#include <Wire.h>
#include "ws2812.h"
#include <SPI.h>

/************************ Configuration ************************/
#define SS_PIN 13
#define Key_Channel  0x03
#define Buzzer_pin 19

// WS2812 LED strip: 6 pixels, default I2C address
PixelStrip strip = PixelStrip(6, 20);

// Key determination upper limit threshold
static byte key_val[] = {190, 205, 220, 235};  // Key 1~Key 4

bool keyOneFlag = true;

/************************ Initialization ************************/
void setup() {
  Serial.begin(115200);

  // Initialize SPI
  SPI1.begin();
  SPI1.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  pinMode(SS_PIN, OUTPUT);

  // Initialize LED strip
  strip.begin(&Wire);
  strip.clear();

  // Initialize buzzer
  pinMode(Buzzer_pin, OUTPUT);
  digitalWrite(Buzzer_pin, HIGH);

  Serial.println("[INFO] Setup done.");
}

/************************ Main Loop ************************/
void loop() {
  byte value = ReadChannel(Key_Channel);       // Read key value from SPI
  byte num = getKeyNum(value);                 // Determine key number
  if (num != 255) {
    switch (num) {
      case 0:
        Serial.println("KEY 1 pressed");
        keyOneFunction();
        break;
      case 1:
        Serial.println("KEY 2 pressed");
        keyTwoFunction();
        break;
      case 2:
        Serial.println("KEY 3 pressed");
        keyThreeFunction();
        break;
      case 3:
        Serial.println("KEY 4 pressed");
        keyFourFunction();
        break;
    }
  }
  delay(300);  // Key debounce
}

/************************ SPI Channel Read ************************/
byte ReadChannel(byte dataToSend) {
  digitalWrite(SS_PIN, LOW);
  byte receivedData = SPI1.transfer(dataToSend);
  digitalWrite(SS_PIN, HIGH);
  return receivedData;
}

/************************ Key Number Determination ************************/
byte getKeyNum(const byte& val) {
  for (int i = 0; i < 4; i++) {
    if (val < key_val[i]) return i;
  }
  return 255;  // No match
}

/************************ Key Functions ************************/
void keyOneFunction() {
  strip.clear();
  if (keyOneFlag) {
    keyOneFlag = false;
    strip.fill(255, 255, 255);
    digitalWrite(Buzzer_pin, LOW);
    delay(500);
    digitalWrite(Buzzer_pin, HIGH);
  } else {
    keyOneFlag = true;
    strip.clear();
    digitalWrite(Buzzer_pin, LOW);
    delay(500);
    digitalWrite(Buzzer_pin, HIGH);
  }
}

void keyTwoFunction() {
  strip.clear();
  // Red light
  strip.setPixelColor(0, strip.Color(255, 0, 0, 0));
  strip.setPixelColor(3, strip.Color(255, 0, 0, 0));
  digitalWrite(Buzzer_pin, LOW);
  delay(1000);
  digitalWrite(Buzzer_pin, HIGH);
  strip.clear();
  delay(1000);

  // Green light
  strip.setPixelColor(1, strip.Color(0, 255, 0, 0));
  strip.setPixelColor(4, strip.Color(0, 255, 0, 0));
  digitalWrite(Buzzer_pin, LOW);
  delay(500);
  digitalWrite(Buzzer_pin, HIGH);
  delay(500);
  strip.clear();
  delay(1000);

  // Yellow light
  strip.setPixelColor(2, strip.Color(255, 255, 0, 0));
  strip.setPixelColor(5, strip.Color(255, 255, 0, 0));
  digitalWrite(Buzzer_pin, LOW);
  delay(200);
  digitalWrite(Buzzer_pin, HIGH);
  delay(300);
  digitalWrite(Buzzer_pin, LOW);
  delay(500);
  digitalWrite(Buzzer_pin, HIGH);
  strip.clear();
}

void keyThreeFunction() {
  for (int j = 0; j < 6; j++) {
    uint32_t randColor = random(0, 16777215);
    strip.setPixelColor(j, randColor);
    delay(500);
    strip.clear();
  }
}

void keyFourFunction() {
  static unsigned long color = 0x0;
  for (int j = 0; j < 5; j++) {
    for (int k = 0; k < 6; k++) {
      for (int l = 0; l < 100; l++) {
        strip.setPixelColor(k, color++);
      }
    }
  }
}    