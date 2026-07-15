#define Buzzer_pin 6

void setup() {
  Serial.begin(9600);
  /* Set the pin controlling the buzzer to the output state */
  pinMode(Buzzer_pin, OUTPUT);
  /* Since the buzzer is inverted control, the control level needs to be set high after power-up */
  digitalWrite(Buzzer_pin, HIGH);
}

void loop() {
  delay(1000);
  digitalWrite(Buzzer_pin, LOW);
  delay(1000);
  digitalWrite(Buzzer_pin, HIGH);
}
