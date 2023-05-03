#include <Arduino.h>

// Define pins for custom SPI communication
const int SDA_PIN = 51; // MOSI
const int SCK_PIN = 52; // SCK
const int MISO_PIN = 50; // MISO
const int RST_PIN = 5; // RST
const int SS_PIN = 53; // SS (slave select)

const int UNLOCK_PIN = 8; // Unlock signal pin

// Add your authorized key here
const byte AUTHORIZED_KEY[4] = {0x12, 0x34, 0x56, 0x78};

void setup() {
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);

  pinMode(UNLOCK_PIN, OUTPUT);

  digitalWrite(SS_PIN, HIGH); // Deselect the RC522
  digitalWrite(SCK_PIN, LOW); // Set clock to low

  rc522_init();
}

void loop() {
  byte received_key[4];

  // Read RFID tag and store the key in the received_key array
  if (read_rfid_tag(received_key)) {
    if (keys_match(received_key, AUTHORIZED_KEY)) {
      unlock_system();
    } else {
      deny_access();
    }
  }
}

// Checks if two keys are equal
bool keys_match(const byte *key1, const byte *key2) {
  for (int i = 0; i < 4; i++) {
    if (key1[i] != key2[i]) {
      return false;
    }
  }
  return true;
}

// Unlocks the system
void unlock_system() {
  digitalWrite(UNLOCK_PIN, HIGH); // Send the unlock signal
}

// Denies access
void deny_access() {
  digitalWrite(UNLOCK_PIN, LOW); // Send the lock signal
}

// Reads an RFID tag and stores the key in the provided array
bool read_rfid_tag(byte *received_key) {
  // Return true if a tag was successfully read, false otherwise
  return false;
}
