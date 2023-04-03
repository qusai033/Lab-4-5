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
  if(activateTimer == COUNTDOWNFALSE){
  activateTimer = COUNTDOWNTRUE;
  }
}
