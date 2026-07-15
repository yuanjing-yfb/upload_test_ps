#define TOUCH_PIN 20

static int oldState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_PIN, INPUT);
}

void loop() {
  byte state = digitalRead(TOUCH_PIN);
  if( state && oldState != state ) {
    Serial.println("[-] Touch!");
    oldState = state;
  } else if( !state && oldState != state ) {
    Serial.println("[-] No Touch!");
    oldState = state;
  }
  delay(20);
}
