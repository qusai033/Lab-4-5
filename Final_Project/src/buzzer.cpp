// define the pin connected to the buzzer
#define BUZZER_PIN 8

void buzzer_setup() {
  // initialize the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
}

void buzzer_on() {
  // turn on the buzzer
  digitalWrite(BUZZER_PIN, HIGH);
}

void buzzer_off() {
  // turn off the buzzer
  digitalWrite(BUZZER_PIN, LOW);
}
