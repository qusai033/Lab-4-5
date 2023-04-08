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
  wait_press, debounce_press, wait_release, debounce_release
} state;

volatile state curState = wait_press;

stateType activateTimer = COUNTDOWNFALSE;

int main(){
    initTimer0();
    initTimer1();
    initSwitchPD0();
    initSegments();
    int segment = 9;
  sei(); // Enable global interrupts.
  
// while loop
 while (1) { 
  if(activateTimer == COUNTDOWNTRUE){
setSegments(segment);
delayS(1);
segment--;
    if(segment == -1){
      segment = 9;
      activateTimer = COUNTDOWNFALSE;
    } 
 }
 else{

 }
 }
  return 0;
}

ISR(INT0_vect){
  switch (curState)
  {
    case wait_press:
      curState = debounce_press;
      break;
    case debounce_press:
      curState = wait_release;
      if(activateTimer == COUNTDOWNFALSE){
        activateTimer = COUNTDOWNTRUE;
      }
      curState = debounce_release;
      break;
    // change the delay in wait release
    case wait_release:
      break;
    case debounce_release:
      curState = wait_press;
      break;
    default:
      curState = wait_press;
  }
}
