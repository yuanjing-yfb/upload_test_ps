#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 9
#define RST_PIN 0xff

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
char tag_uid_hex[15] = {};  // Used to save the converted UID string

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key: "));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // Only process if UID is different from before
  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3]) {

    Serial.println(F("A new card has been detected."));

    // Save current UID
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    // Display UID (hexadecimal)
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    // Construct UID hexadecimal string for comparison
    if (rfid.uid.size == 4) {
      sprintf(tag_uid_hex, "%02X%02X%02X%02X",
              rfid.uid.uidByte[0],
              rfid.uid.uidByte[1],
              rfid.uid.uidByte[2],
              rfid.uid.uidByte[3]);
    } else if (rfid.uid.size == 7) {
      sprintf(tag_uid_hex, "%02X%02X%02X%02X%02X%02X%02X",
              rfid.uid.uidByte[0],
              rfid.uid.uidByte[1],
              rfid.uid.uidByte[2],
              rfid.uid.uidByte[3],
              rfid.uid.uidByte[4],
              rfid.uid.uidByte[5],
              rfid.uid.uidByte[6]);
    }

    // Print welcome message
    if (strcmp(tag_uid_hex, "4A796959") == 0) {
      Serial.println("Hello CrowPi3");
    } else if (strcmp(tag_uid_hex, "EBFB3434") == 0) {
      Serial.println("Hello Elecrow");
    } else if (strcmp(tag_uid_hex, "3BE13934") == 0) {
      Serial.println("Hello World");
    } else {
      Serial.print("Unrecognized card UID: ");
      Serial.println(tag_uid_hex);
    }
  } else {
    Serial.println(F("Card read previously."));
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
