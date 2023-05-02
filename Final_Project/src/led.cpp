#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void initLED(){
  DDRE |= (1 << DDE3);
  DDRH |= (1 << DDH3);
}

void alternateLED(bool unlocked){
  if(unlocked){
    PORTE |= (1 << PORTE3);
    PORTH &= ~(1 << PORTH3);
  }
  else{
    PORTH |= (1 << PORTH3);
    PORTE &= ~(1 << PORTE3);
  }
}
