#include <SPI.h>

#define SS_PIN 13
#define RockerX_Channel 0x02
#define RockerY_Channel 0x01

#define CENTER_X 127
#define CENTER_Y 129
#define DEADZONE 15

String lastDirection = "center";

byte ReadChannel(byte channel) {
  digitalWrite(SS_PIN, LOW);
  byte value = SPI.transfer(channel);
  digitalWrite(SS_PIN, HIGH);
  return value;
}

// Determine the direction of maximum deviation to avoid triggering both directions simultaneously
String getDirection(byte x, byte y) {
  int dx = x - CENTER_X;
  int dy = y - CENTER_Y;

  if (abs(dx) < DEADZONE && abs(dy) < DEADZONE) {
    return "center";
  }

  // Compare which direction has a greater offset
  if (abs(dx) > abs(dy)) {
    return dx > 0 ? "left" : "right";
  } else {
    return dy > 0 ? "up" : "down";
  }
}

void setup() {
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  pinMode(SS_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  byte x_value = ReadChannel(RockerX_Channel);
  delay(100);
  byte y_value = ReadChannel(RockerY_Channel);
  delay(100);

  String direction = getDirection(x_value, y_value);
  if (direction != lastDirection) {
    lastDirection = direction;
    if (direction == "up") Serial.println("upward");
    else if (direction == "down") Serial.println("downward");
    else if (direction == "left") Serial.println("towards the left");
    else if (direction == "right") Serial.println("towards the right");
  }

  delay(100);
}
