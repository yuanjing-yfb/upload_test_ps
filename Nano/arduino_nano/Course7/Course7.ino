#define Buzzer_pin 6
unsigned int volume = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  for (volume = 0; volume <= 255; volume++) {
      analogWrite(Buzzer_pin, volume);
      delay(10);
  }
  for (volume = 255; volume >= 0; volume--) {
      analogWrite(Buzzer_pin, volume);
      delay(10);
  }
}
