// PIR motion sensor integration with Arduino board

// Define the PIR sensor pin
const int pirPin = 2;

// Define the buzzer pin
const int buzzerPin = 3;

// Variable to store PIR sensor state
int pirState = 0;

void setup() {
  // Initialize serial communication at 9600 baudrate
  Serial.begin(9600);
  
  // Initialize PIR sensor pin as input
  pinMode(pirPin, INPUT);
  
  // Initialize buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Read PIR sensor state
  pirState = digitalRead(pirPin);
  
  if (pirState == HIGH) {
    // Motion detected
    Serial.println("Motion Detected!");
    digitalWrite(buzzerPin, HIGH); // Turn on buzzer
    delay(500); // Wait for half a second
  } else {
    // No motion detected
    Serial.println("No Motion");
    digitalWrite(buzzerPin, LOW); // Turn off buzzer
    delay(500); // Wait for half a second
  }
} 
