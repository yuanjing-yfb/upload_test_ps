int buzzerPin = 19;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
}

void loop() {
  // change the analog out value:
  Serial.println("[-] buzzer able!" );
  digitalWrite(buzzerPin, LOW);
  delay(1000);
  Serial.println("[-] buzzer unable!");
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
}
