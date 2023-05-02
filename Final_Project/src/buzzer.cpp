#include "buzzer.h"
#include <avr/io.h>


void initBuzzer(){
    DDRG |= (1 << DDG5);

}
void buzzerOn(bool isOn){
    if(isOn){
        PORTG |= (1 << PORTG5);
    }
    else{
        PORTG &= ~(1 << PORTG5);
    }
}
