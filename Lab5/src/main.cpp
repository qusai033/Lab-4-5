#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "i2c.h"
#include "spi.h"

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
  
}


ISR(INT0_vect){

}