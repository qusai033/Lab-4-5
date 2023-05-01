// PIR motion sensor and buzzer integration with Arduino

// Define the PIR sensor pin, LED pin, and buzzer pin
const int PIRSensorPin = 2;
const int LEDPin = 13;
const int buzzerPin = 3;

// Declare the variables
int motionState = LOW;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Set the PIR sensor pin as INPUT, LED pin as OUTPUT, and buzzer pin as OUTPUT
  pinMode(PIRSensorPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Allow the sensor to stabilize for a moment
  delay(2000);
}

void loop() {
  // Read the state of the PIR sensor
  motionState = digitalRead(PIRSensorPin);

  // Check if motion is detected
  if (motionState == HIGH) {
    // Turn the LED on, activate the buzzer, and print the message to the Serial Monitor
    digitalWrite(LEDPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Motion detected!");
    delay(1000);
  } else {
    // Turn the LED off, deactivate the buzzer, and print the message to the Serial Monitor
    digitalWrite(LEDPin, LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("No motion detected.");
    delay(1000);
  }
}
