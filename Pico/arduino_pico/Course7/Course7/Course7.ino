#define Buzzer_pin 19

int volume = 0;
bool increasing = true;

void setup() {
  pinMode(Buzzer_pin, OUTPUT);
}

void loop() {
  analogWrite(Buzzer_pin, volume);  // PWM duty cycle control

  delay(40);  // Control volume change rhythm, larger value = slower/softer

  if (increasing) {
    volume += 2;  // More noticeable change
    if (volume >= 180) {  // Medium loudness, avoid using 255
      increasing = false;
    }
  } else {
    volume -= 2;
    if (volume <= 0) {    // Mute, completely silent
      increasing = true;
    }
  }
}