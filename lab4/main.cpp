#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "segments.h"
#include <avr/interrupt.h>

/*
 * Define a set of states that can be used in the state machine using an enum.
 */

typedef enum{
COUNTDOWNTRUE,
COUNTDOWNFALSE
} stateType;

typedef enum states_ {
  WAIT_RELEASE,
WAIT_PRESS,
DEBOUNCE_PRESS,
DEBOUNCE_RELEASE
} state;

stateType activateTimer = COUNTDOWNFALSE;

int main(){
    initTimer0();
    initTimer1();
    initSwitchPD0();
    initPWMTimer3();
    initADC0();
    init7Segments();
    Serial.begin(9600);
    int segment = 9;
  sei(); // Enable global interrupts.
  
// while loop
 while (1) { 
  if(activateTimer == COUNTDOWNTRUE){
    changeDutyCycle(2.5);
    if(segment == -1){
      segment = 9;
    } 
    turnOn7segments(segment);
    delayS(1);
    segment--;
    if(segment == -1){
      turnOn7segments(segment);
      activateTimer = COUNTDOWNFALSE;
    } 
  }
  else{
    //Serial.println(ADC/200.0);
    changeDutyCycle(ADC/200.0);
  }
 }
  return 0;
}

ISR(INT0_vect){
  if(activateTimer == COUNTDOWNFALSE){
state state = WAIT_PRESS;
bool debouncing = true;

while(debouncing){
if(state == WAIT_PRESS){
  delayMs(1);
state = DEBOUNCE_PRESS;
}
else if(state == DEBOUNCE_PRESS){
  delayMs(1);
  state = WAIT_RELEASE;
}
else if(state == WAIT_RELEASE){
  delayMs(1);
state = DEBOUNCE_RELEASE;
  activateTimer = COUNTDOWNTRUE;
}
else if(state == DEBOUNCE_RELEASE){
  delayMs(1);
debouncing = false;
}
}
}
}