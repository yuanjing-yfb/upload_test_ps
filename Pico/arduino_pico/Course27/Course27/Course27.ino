#include <SPI.h>

// Define SPI chip select pin
#define SS_PIN 13

#define Voltage_Channel  0x04

byte ReadChannel(byte dataToSend) {
  // Select slave device
  digitalWrite(SS_PIN, LOW);
  // Send data and read return value
  byte receivedData = SPI1.transfer(dataToSend);
  // Release slave device
  digitalWrite(SS_PIN, HIGH);
  return receivedData;
}

void setup() {
  // Initialize SPI
  SPI1.begin();
  SPI1.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  
  Serial.begin(115200);
}

void loop() {
  // Read ADC value of channel 4
  byte voltageRaw = ReadChannel(Voltage_Channel);
  
  // Convert ADC raw value to voltage (assuming 8-bit, reference voltage is 3.3V)
  float voltage = (voltageRaw / 255.0) * 3.3;

  // Print the result
  Serial.print("Voltage Raw Value: ");
  Serial.print(voltageRaw);
  Serial.print(" \t Voltage: ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  delay(1000);
}
