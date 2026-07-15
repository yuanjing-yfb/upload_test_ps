const int hallPin = 21; 

void setup() {
  pinMode(hallPin, INPUT); 
  Serial.begin(9600); 
}

void loop() {
  int hallState = digitalRead(hallPin); 
  if (hallState == HIGH) {
    Serial.println("Non-magnetic field"); 
  } else {
    Serial.println("Magnetic field detected");
  }
  delay(1000);
}
