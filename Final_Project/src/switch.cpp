#include "switch.h"
#include <avr/io.h>


void initSwitchPB4(){
  DDRB &= ~(1 << DDB4);
  PORTB |= (1 << PORTB4);

  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT4);
}
