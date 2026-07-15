#include "DHT20.h"
#include "LED.hpp"

DHT20 DHT;
PixelStrip strip = PixelStrip(6, 50);
int Temp = 0;
int Humidity = 0;

int TempPresetValue = 30;
int HumidityPresetValue =  50;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  DHT.begin();
  strip.begin();
  delay(1000);
}

void loop() {
  if (millis() - DHT.lastRead() >= 1000) {
    //  READ DATA
    uint32_t start = micros();
    int status = DHT.read();
    uint32_t stop = micros();

    Humidity = DHT.getHumidity();
    Temp = DHT.getTemperature();
    Serial.print("Humidity: ");
    Serial.print(Humidity);
    Serial.print("\tTemp: ");
    Serial.println(Temp);

    if (Temp > TempPresetValue)
    {
      strip.setPixelColor(1, strip.Color(255, 0, 0, 0));
      strip.setPixelColor(0, strip.Color(0, 0, 0, 0));
    }
    else
    {
      strip.setPixelColor(0, strip.Color(0, 255, 0, 0));
      strip.setPixelColor(1, strip.Color(0, 0, 0, 0));
    }
    if (Humidity > HumidityPresetValue)
    {
      strip.setPixelColor(3, strip.Color(255, 0, 0, 0));
      strip.setPixelColor(2, strip.Color(0, 0, 0, 0));
    }
    else
    {
      strip.setPixelColor(2, strip.Color(0, 255, 0, 0));
      strip.setPixelColor(3, strip.Color(0, 0, 0, 0));
    }
  }
}
