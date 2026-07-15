int relayPin = A3;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  Serial.println("relay active high");
  digitalWrite(relayPin, HIGH);
  delay(500);
  Serial.println("relay active low");
  digitalWrite(relayPin, LOW);
  delay(500);
}
