
#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
void initTimer0();
void initTimer1();
void delayS(unsigned int delay);
void delayMs(unsigned int delay); 

#endif
