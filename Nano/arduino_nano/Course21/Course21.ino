#include <Adafruit_MCP3008.h>
#include "LED.hpp"


/************************ configuration ************************/
#define SS_PIN 10
#define Key_Channel  0x03

static byte key_val[] = {190, 205, 220, 235};  //Keys 1 to 4

PixelStrip strip = PixelStrip(6, 20);

void setup() {
  Serial.begin(115200);

  // Initialize SPI
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  pinMode(SS_PIN, OUTPUT);

  // Initialize the light strip
  strip.begin();
  strip.clear();
}
void loop() {
  byte value = ReadChannel(Key_Channel);       // Read key values from SPI
  byte num = getKeyNum(value);                 // Determine the key number
  if (num != 255) {
    switch (num) {
      case 0:
        Serial.println("KEY 1 pressed");
        strip.setPixelColor(0, strip.Color(255, 0, 0, 0));
        strip.setPixelColor(1, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(2, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(3, strip.Color(0, 0, 0, 0));
        break;
      case 1:
        Serial.println("KEY 2 pressed");
        strip.setPixelColor(1, strip.Color(0, 255, 0, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(2, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(3, strip.Color(0, 0, 0, 0));
        break;
      case 2:
        Serial.println("KEY 3 pressed");
        strip.setPixelColor(2, strip.Color(0, 0, 255, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(1, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(3, strip.Color(0, 0, 0, 0));
        break;
      case 3:
        Serial.println("KEY 4 pressed");
        strip.setPixelColor(3, strip.Color(125, 0, 125, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(1, strip.Color(0, 0, 0, 0));
        strip.setPixelColor(2, strip.Color(0, 0, 0, 0));
        break;
      default:
        break;
    };
  }
  delay(200);
}

/************************ SPI channel reading ************************/
byte ReadChannel(byte dataToSend) {
  digitalWrite(SS_PIN, LOW);
  byte receivedData = SPI.transfer(dataToSend);
  digitalWrite(SS_PIN, HIGH);
  return receivedData;
}

/************************ Button number judgment ************************/
byte getKeyNum(const byte& val) {
  for (int i = 0; i < 4; i++) {
    if (val < key_val[i]) return i;
  }
  return 255; 
}