#define PIR_PIN 6

static int oldState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  byte state = digitalRead(PIR_PIN);
  if( state && oldState != state ) {
    Serial.println("[-] Motion detected!");
    oldState = state;
  } else if( !state && oldState != state ) {
    Serial.println("[-] No Motion!");
    oldState = state;
  }
  delay(20);
}
