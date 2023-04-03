#include "timer.h"

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
TCCR1A &= ~(1 << WGM10);
TCCR1A &= ~(1 << WGM11);
TCCR1B|= (1 << WGM12);
TCCR1B &= ~(1 << WGM13);

}
void delayS(unsigned int delay){
    unsigned int delayCnt = 0;
    OCR1A = 15625; 

    TIFR1 |= (1 << OCF1A); //get clock ready to start
    TCNT1 = 0;  

    TCCR1B |= ((1 << CS12)|(1 << CS10));
    TCCR1B &= ~(1 << CS11); //Prescalar of 1024

    while (!(TIFR1 & (1 << OCF1A)));  //While running
    while (delayCnt < delay) {
        if (TIFR1 & (1 << OCF1A)) { //count to set delay using a 1 second timer
            delayCnt++;
            TIFR1 |= (1 << OCF1A);
        }
    }
    TCCR1B &= ~((1 << CS12)|(1 << CS10)|(1 << CS11)); //turn timer off
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
TCCR0A &= ~(1 << WGM00);
TCCR0A |=  (1 << WGM01);
TCCR0B &= ~(1 << WGM02);

OCR0A = 249; //counter of 249
}

void delayMs(unsigned int delay){
   unsigned int delayCnt = 0;
    TCNT0 = 0; //get clock ready
    TIFR0 |= (1 << OCF0A);
    TCCR0B |=  ((1 << CS01) | (1 << CS00));
    TCCR0B &= ~(1 << CS02); //Start clock
    while (delayCnt < delay) {
        if (TIFR0 & (1 << OCF0A)) { //count to set delay using a 1 millisecond timer
            delayCnt++;
            TIFR0 |= (1 << OCF0A);
        }
    }
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); //turn off clock
}

