#include <Servo.h>

// Define the servo motor pin
const int servoPin = 9;

// Create an instance of the Servo class
Servo myservo;

void setup() {
  // Initialize the servo motor pin as output
  myservo.attach(servoPin);
}

void loop() {
  // Move the servo motor to 0 degrees (locked position)
  myservo.write(0);
  delay(1000); // Wait for 1 second
  
  // Move the servo motor to 90 degrees (unlocked position)
  myservo.write(90);
  delay(1000); // Wait for 1 second
}
