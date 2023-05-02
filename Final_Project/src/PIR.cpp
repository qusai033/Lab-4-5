#include "PIR.h"
#include <avr/io.h>


void initPIR(){
    DDRH &= ~(1<<DDH4);     //initialized as input
    PORTH |= (1<<PORTH4);   //enable pull-up resistor pin 21
}
