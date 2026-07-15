/*
 * Lesson 2: Button Detection
 *
 * Function:
 * Rotary encoder button → Serial print short press / long press / double click
 * Boot key              → Serial print short press / long press / double click
 *
 * Hardware: ESP32-Watch-Display
 * Development Environment: Arduino IDE 2.3.6+, ESP32 Board v3.3.3
 *
 * Dependent Library: OneButton (Uses the main project libraries/OneButton directory)
 */

#include <OneButton.h>
#include "config.h"

// Button objects (Consistent with main project: second parameter true = enable internal pull-up resistor)
OneButton encoderKey(PIN_EC_BTN, true);
OneButton bootKey(PIN_BOOT_KEY, true);

void keyInit() {
  // Rotary encoder button callbacks
  encoderKey.attachClick([]() {
    Serial.println("Rotary Encoder Button: Short Press");
  });
  encoderKey.attachLongPressStart([]() {
    Serial.println("Rotary Encoder Button: Long Press");
  });
  encoderKey.attachDoubleClick([]() {
    Serial.println("Rotary Encoder Button: Double Click");
  });

  // Boot key callbacks
  bootKey.attachClick([]() {
    Serial.println("Boot Key: Short Press");
  });
  bootKey.attachLongPressStart([]() {
    Serial.println("Boot Key: Long Press");
  });
  bootKey.attachDoubleClick([]() {
    Serial.println("Boot Key: Double Click");
  });
}

void setup() {
  Serial.begin(115200);

  keyInit();

  Serial.println();
  Serial.println("====== Button Detection ======");
  Serial.println("Rotary Encoder Button / Boot Key");
  Serial.println("Short Press / Long Press / Double Click → Serial Print");
  Serial.println("========================================");
}

void loop() {
  // Must call tick() periodically 
  // so OneButton can scan and identify button actions
  encoderKey.tick();
  bootKey.tick();

  delay(20);  // Consistent with the main project key_task polling interval
}
