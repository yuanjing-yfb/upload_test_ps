#define flame_pin A2

static byte oldState = 0;
static byte flameState = 0;

#define buzzerPin  6

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(flame_pin, INPUT);
}

void loop() {
  flameState = digitalRead(flame_pin);
  if (!flameState && oldState != flameState) {
    Serial.println("[-] Flame Detect!");
    oldState = flameState;
    digitalWrite(buzzerPin, LOW);
    delay(1000);
  } else if (flameState && oldState != flameState) {
    Serial.println("[-] Flame not Detect!");
    oldState = flameState;
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
  }
}
