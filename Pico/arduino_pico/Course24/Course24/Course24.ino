#include <Arduino.h>
#include "MFRC522.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define SS_PIN 11

typedef struct {
  const char* name;
  int rst_pin;
  int ss_pin;
} sensor_t;

sensor_t RFID_SENSORS[] = {{"RFID_1", UINT8_MAX, SS_PIN}};

const int sckPin = 14;
const int mosiPin = 15;
const int misoPin = 12;

const int RFID_COUNTS = ARRAY_SIZE(RFID_SENSORS);
MFRC522 mfrc522[RFID_COUNTS];   // Create MFRC522 instance.
char tag_uid_hex[15] = {}; // Define a 15 byte character array to store the hexadecimal string of RFID tag UID

void setup() {
  Serial.begin(9600);
  SPI1.setSCK(sckPin);
  SPI1.setTX(mosiPin);
  SPI1.setRX(misoPin);
  SPI1.begin();
  for (uint8_t reader = 0; reader < RFID_COUNTS; reader++) {
    mfrc522[reader].PCD_Init(RFID_SENSORS[reader].ss_pin, RFID_SENSORS[reader].rst_pin); // Init each MFRC522 card
    Serial.print(RFID_SENSORS[reader].name);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

void loop() {
  for (uint8_t reader = 0; reader < RFID_COUNTS; reader++) {
    // Look for new cards
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      String tag_name = RFID_SENSORS[reader].name;
      byte tag_uid[7] = {}; // rfid uid length is 4 or 7 bytes
      int tag_uid_length = mfrc522[reader].uid.size;
      memcpy(tag_uid, mfrc522[reader].uid.uidByte, tag_uid_length); // shallow copy of uid
      if (tag_uid_length == 4)
        sprintf(tag_uid_hex, "%02X%02X%02X%02X", tag_uid[0], tag_uid[1], tag_uid[2], tag_uid[3]);
      else
        sprintf(tag_uid_hex, "%02X%02X%02X%02X%02X%02X%02X", tag_uid[0], tag_uid[1], tag_uid[2], tag_uid[3], tag_uid[4], tag_uid[5], tag_uid[6]);
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
      Serial.print(RFID_SENSORS[reader].name);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      Serial.println(tag_uid_hex);
      if (strcmp(tag_uid_hex, "4A796959") == 0) {
        Serial.println("Hello CrowPi3");
      }
      else if(strcmp(tag_uid_hex, "EBFB3434") == 0)
      {
        Serial.println("Hello Elecrow");
      }
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader
}
