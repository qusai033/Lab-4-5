#include <SPI.h>
#include <MFRC522.h>

// Define the pins connected to the RC522 module
#define SS_PIN 10
#define RST_PIN 9

// Create an instance of the MFRC522 class
MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() {
  // Initialize serial communication
  Serial.begin(9600); 
  
  // Initialize the RC522 module
  SPI.begin(); 
  rfid.PCD_Init(); 
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Get the UID of the card
    String content= "";
    for (byte i = 0; i < rfid.uid.size; i++) {
       content.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
       content.concat(String(rfid.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    
    // Print the UID to the serial monitor
    Serial.print("UID tag :"); 
    Serial.println(content);
  }
  delay(1000);
}
