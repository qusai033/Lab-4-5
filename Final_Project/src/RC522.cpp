// Include the necessary libraries
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// Define the RFID module pins
#define SDA_PIN 10
#define RST_PIN 9
#define SCK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11

// Define pins for other components
const int PIRSensorPin = 2;
const int servoPin = 6;
const int LEDPin = 13;
const int greenLEDPin = 4;
const int buzzerPin = 3;

// Create instances of the MFRC522 and Servo libraries
MFRC522 rfid(SDA_PIN, RST_PIN);
Servo servo;

// Set the valid RFID card UID
const byte validCardUID[] = {0x12, 0x34, 0x56, 0xAB}; // Replace with the UID of your valid RFID card

// Declare variables
int lockPosition = 0;   // Locked position for the servo
int unlockPosition = 90; // Unlocked position for the servo
unsigned long unlockDuration = 5000; // Duration (in ms) to keep the door unlocked before locking it again
int motionState = LOW;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize SPI communication
  SPI.begin();
  rfid.PCD_Init();

  // Set pins for other components
  pinMode(PIRSensorPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Attach the servo motor to the specified pin and set it to the locked position
  servo.attach(servoPin);
  servo.write(lockPosition);

  Serial.println("Ready to scan RFID cards...");
}

void loop() {
  // Check for motion detection
  motionState = digitalRead(PIRSensorPin);
  if (motionState == HIGH) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Check if a new RFID card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Check if the RFID card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Check if the scanned RFID card matches the valid RFID card
  bool isCardValid = true;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != validCardUID[i]) {
      isCardValid = false;
      break;
    }
  }

  if (isCardValid) {
    // Unlock the door and turn on the green LED
    Serial.println("Valid RFID card detected! Unlocking the door...");
    servo.write(unlockPosition);
    digitalWrite(greenLEDPin, HIGH);
    delay(unlockDuration);

    // Lock the door and turn off the green LED
    Serial.println("Locking the door...");
    servo.write(lockPosition);
    digitalWrite(greenLEDPin, LOW);
  } else {
    Serial.println("Invalid RFID card. Access denied.");
  }

  // Halt the RFID card
  rfid.PICC_HaltA();
}
