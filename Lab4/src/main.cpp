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

typedef enum{ COUNTDOWNTRUE, COUNTDOWNFALSE} stateType;

typedef enum states_ {
  WAIT_RELEASE,
  WAIT_PRESS,
  DEBOUNCE_PRESS,
  DEBOUNCE_RELEASE
} state;

// Initialize activateTimer to COUNTDOWNFALSE
stateType activateTimer = COUNTDOWNFALSE;

int main(){
    // Initialize various modules and global interrupts
    initTimer0();
    initTimer1();
    initSwitchPD0();
    initPWMTimer3();
    initADC0();
    init7Segments();
    Serial.begin(9600);
    int segment = 9;
    sei(); // Enable global interrupts.
  
    // Main while loop
    while (1) { 
        // If the timer has been activated
        if(activateTimer == COUNTDOWNTRUE){
            // Set duty cycle to 2.5% and display countdown on 7-segment display
            changeDutyCycle(2.5);
            if(segment == -1){
                segment = 9;
            } 
            turnOn7segments(segment);
            delayS(1);
            segment--;

            // If countdown is complete, turn off 7-segment display and deactivate timer
            if(segment == -1){
                turnOn7segments(segment);
                activateTimer = COUNTDOWNFALSE;
            } 
        }
        else{
            // Read ADC and set duty cycle accordingly
            changeDutyCycle(ADC/200.0);
        }
    }
    return 0;
}


ISR(INT0_vect){
  // Check if timer is not currently counting down
  if(activateTimer == COUNTDOWNFALSE){
    // Initialize the state variable and debouncing variable
    state state = WAIT_PRESS;
    bool debouncing = true;

    // Enter debouncing loop
    while(debouncing){
      // Check state and wait for button press
      if(state == WAIT_PRESS){
        delayMs(1);
        state = DEBOUNCE_PRESS;
      }
      // Check state and wait for button release
      else if(state == DEBOUNCE_PRESS){
        delayMs(1);
        state = WAIT_RELEASE;
      }
      // Check state and prepare to activate the timer
      else if(state == WAIT_RELEASE){
        delayMs(1);
        state = DEBOUNCE_RELEASE;
        activateTimer = COUNTDOWNTRUE;
      }
      // Check state and finish debouncing
      else if(state == DEBOUNCE_RELEASE){
        delayMs(1);
        debouncing = false;
      }
    }
  }
}
