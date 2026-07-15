#define SOUND_PIN 5

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_PIN, INPUT);
}

void loop() {
  if( digitalRead(SOUND_PIN) ) {
    Serial.println("[-] Detect Sound!");
  }
  delay(20);
}
