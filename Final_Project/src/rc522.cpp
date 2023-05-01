#include <SPI.h>
#include <MFRC522.h>

// define the pins connected to the RC522 module
#define SS_PIN 10
#define RST_PIN 9

// create an instance of the MFRC522 class
MFRC522 mfrc522(SS_PIN, RST_PIN);

void rc522_setup() {
  // initialize the RC522 module
  SPI.begin();
  mfrc522.PCD_Init();
}

bool rc522_read_uid(String& uid) {
  // check for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // get the UID of the card
    uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // halt the RC522 module to prevent further reading of this card
    mfrc522.PICC_HaltA();

    // stop the encryption on the card to save power
    mfrc522.PCD_StopCrypto1();

    return true;
  }

  return false;
}
