int vibPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(vibPin, OUTPUT);
}

void loop() {
  Serial.println("active high");
  digitalWrite(vibPin, HIGH);
  delay(500);
  Serial.println("active low");
  digitalWrite(vibPin, LOW);
  delay(500);
}
