// define the pins connected to the LEDs
#define GREEN_LED_PIN 5
#define RED_LED_PIN 6

void led_setup() {
  // initialize the LED pins as outputs
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void green_led_on() {
  // turn on the green LED and turn off the red LED
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
}

void red_led_on() {
  // turn on the red LED and turn off the green LED
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
}

void led_off() {
  // turn off both LEDs
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}
