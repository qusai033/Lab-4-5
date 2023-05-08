// Include the necessary libraries
#include <SPI.h>
#include "PIR.h"
#include "led.h"
#include "buzzer.h"
#include "pwm.h"
#include <MFRC522.h>

// Define the RFID module pins
#define SDA_PIN 53
#define RST_PIN 5
#define SCK_PIN 52
#define MISO_PIN 50
#define MOSI_PIN 51

Define pins for other components
const int PIRSensorPin = 7;
const int servoPin = 3;
const int redLEDPin = 6;
const int greenLEDPin = 8;
const int buzzerPin = 4;

// Create instances of the MFRC522 and Servo libraries
MFRC522 rfid(SDA_PIN, RST_PIN);

// Set the valid RFID card UID
const byte validCardUID[] = {0x41, 0xFD, 0x60, 0x22}; // Replace with the UID of your valid RFID card

// Declare variables
int lockPosition = 0;   // Locked position for the servo
int unlockPosition = 90; // Unlocked position for the servo
unsigned long unlockDuration = 5000; // Duration (in ms) to keep the door unlocked before locking it again
int motionState = LOW;

// no library variables
bool locked = true;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize SPI communication
  SPI.begin();
  rfid.PCD_Init();

  // Set pins for other components
  pinMode(PIRSensorPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  
  initPIR();
  initLED();
  initBuzzer();
  initPWMTimer3();
  
  
  setLocked(locked);

  // Attach the servo motor to the specified pin and set it to the locked position
  //servo.attach(servoPin);
  //servo.write(lockPosition);

  Serial.println("Ready to scan RFID cards...");
}

void loop() {
  // Check for motion detection
  motionState = digitalRead(PIRSensorPin);
  if (motionState == HIGH) {
    buzzerOn(false);
    digitalWrite(buzzerPin, HIGH);
  } else {
    buzzerOn(true);
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
    setLocked(false);
    digitalWrite(greenLEDPin, HIGH);
    alternateLED(true);
    delay(unlockDuration);

    // Lock the door and turn off the green LED
    Serial.println("Locking the door...");
    _delay_ms(1000);
    //servo.write(lockPosition);
    setLocked(true);
    //digitalWrite(greenLEDPin, LOW);
    alternateLED(false);
  } else {
    // Door stay locked and turn on the red LED 
    Serial.println("Invalid RFID card. Access denied.");
    digitalWrite(redLEDPin, HIGH);
    alternateLED(false);
    delay(unlockDuration-2000);

    // door is still locked and turn off the red LED
    Serial.println("Stay outside :D ...");
    setLocked(true);
    digitalWrite(redLEDPin, LOW);
  }
  // Halt the RFID card
  rfid.PICC_HaltA();
}
