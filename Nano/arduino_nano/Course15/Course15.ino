#define TILT_PIN 7

static int oldState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TILT_PIN, INPUT);
}

void loop() {
  byte state = digitalRead(TILT_PIN);
  if( state && oldState != state ) {
    Serial.println("[-] Lean to the left");
    oldState = state;
  } else if( !state && oldState != state ) {
    Serial.println("[-] Lean to the right");
    oldState = state;
  }
  delay(500);
}
