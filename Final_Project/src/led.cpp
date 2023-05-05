#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void initLED(){
  DDRH |= (1 << DDH5);
  DDRH |= (1 << DDH3);
}

void alternateLED(bool unlocked){
  if(unlocked){
    PORTH |= (1 << PORTH5);
    PORTH &= ~(1 << PORTH3);
  }
  else{
    PORTH |= (1 << PORTH3);
    PORTH &= ~(1 << PORTH5);
  }
}
