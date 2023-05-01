// Include the necessary libraries
#include <Servo.h>

// Define the servo motor pin and LED pin
const int servoPin = 9;
const int LEDPin = 13;

// Create an instance of the Servo library
Servo servo;

// Declare variables
int servoPosition = 0; // Initial position for the servo

void setup() {
  // Attach the servo motor to the specified pin
  servo.attach(servoPin);

  // Set the LED pin as OUTPUT
  pinMode(LEDPin, OUTPUT);

  // Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (servoPosition = 0; servoPosition <= 180; servoPosition++) {
    servo.write(servoPosition);
    digitalWrite(LEDPin, HIGH); // Turn the LED on
    delay(15); // Adjust the delay to control the servo sweep speed
  }

  // Sweep the servo from 180 to 0 degrees
  for (servoPosition = 180; servoPosition >= 0; servoPosition--) {
    servo.write(servoPosition);
    digitalWrite(LEDPin, LOW); // Turn the LED off
    delay(15); // Adjust the delay to control the servo sweep speed
  }
}
