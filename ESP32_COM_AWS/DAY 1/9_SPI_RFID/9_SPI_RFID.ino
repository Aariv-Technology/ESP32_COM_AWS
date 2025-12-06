#include <SPI.h>      // SPI communication for MFRC522
#include <MFRC522.h>  // RFID library

#define SS_PIN 5   // SDA / SS pin of RFID module
#define RST_PIN 4  // Reset pin of RFID module

MFRC522 rfid(SS_PIN, RST_PIN);  // Create RFID object
byte lastUID[4];                // Stores last scanned 4-byte UID

void setup() {
  Serial.begin(9600);  // Serial monitor for debugging
  SPI.begin();         // Start SPI bus
  rfid.PCD_Init();     // Initialize RFID reader
  Serial.println("Ready to scan RFID cards...");
}

void loop() {

  // Check if any card is placed near the reader
  if (!rfid.PICC_IsNewCardPresent()) return;

  // Try to read the card's UID
  if (!rfid.PICC_ReadCardSerial()) return;

  // Detect the card type (MIFARE, NTAG, etc.)
  MFRC522::PICC_Type type = rfid.PICC_GetType(rfid.uid.sak);

  Serial.print("Card Type: ");
  Serial.println(rfid.PICC_GetTypeName(type));

  // Allow only MIFARE Classic cards
  if (type != MFRC522::PICC_TYPE_MIFARE_MINI && type != MFRC522::PICC_TYPE_MIFARE_1K && type != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Not a MIFARE Classic card!");
    return;
  }

  // -------- CHECK IF CARD IS NEW OR SAME AS LAST TIME --------
  bool isNew = false;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != lastUID[i]) {
      isNew = true;  // UID mismatch → new card
      break;
    }
  }

  if (isNew) {
    Serial.println("New card detected!");

    // Save this UID for next comparison
    for (byte i = 0; i < 4; i++) {
      lastUID[i] = rfid.uid.uidByte[i];
    }

    // -------- PRINT UID IN HEX FORMAT --------
    Serial.print("UID (HEX): ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // -------- PRINT UID IN DECIMAL FORMAT --------
    Serial.print("UID (DEC): ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i]);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Card already scanned earlier.");  // Same card as before
  }

  // Stop communication with this card
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
