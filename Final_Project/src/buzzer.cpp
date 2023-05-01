// define buzzer pin
const int buzzerPin = 3;

void setup() {
  // initialize buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // turn on buzzer for 1 second
  digitalWrite(buzzerPin, HIGH);
  delay(1000);

  // turn off buzzer for 1 second
  digitalWrite(buzzerPin, LOW);
  delay(1000);
}
