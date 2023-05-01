#include <Servo.h>

// create an instance of the Servo class
Servo servo;

// define the pin connected to the servo motor
#define SERVO_PIN 11

void servo_setup() {
  // initialize the servo motor
  servo.attach(SERVO_PIN);
}

void servo_unlock() {
  // unlock the door by rotating the servo motor to 90 degrees
  servo.write(90);
}

void servo_lock() {
  // lock the door by rotating the servo motor back to 0 degrees
  servo.write(0);
}
