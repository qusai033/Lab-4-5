#include <avr/io.h>
#include "pwm.h"
#include <Arduino.h>



void initPWMTimer3(){
    DDRE |= (1<<PORTE3); //set PIN 5 to output  this is for PWM 
    DDRH |= (1<<PORTH3); //set PIN 6 to output  
    //DDRH |= (1<<PORTH4); //set PIN 7 to output = 1A tocontrol motor

    //Timer 3
    //set non-inverting mode: output starts high and then is low
    TCCR3A |= (1<< COM3A1); //set output for OCR to be non-inverting
    TCCR3A &= ~(1<<COM3A0);

    //set WGM bits for PWM mode 7 fast 10 bit

    //Fast PWM for 10 bit top 
    //WGM33 = 0  WGM32 = 1 WGM31= 1 WGM30= 1
    //WGM33 and WGM32 are in TCCR3B
    //WGM31 and WGM30 are in TCCR3A

    TCCR3B &= ~(1<<WGM33);
    TCCR3B |= (1<<WGM32);
    TCCR3A |= (1<<WGM31)|(1<<WGM30);

    /// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
    //TOP = 1023

    //set ps to 1
    //CS32 = 0  CS31 = 0 CS30=1
    //CS are in TCCR3B
    TCCR3B &= ~(1<<CS32);
    TCCR3B &= ~(1<<CS31);
    TCCR3B |=  (1<<CS30);
}


void change_frequency(unsigned int freq_val){
    OCR3A = 16000000 / freq_val;
    PORTH |= (1 << PORTH3);
    delayMicroseconds(1000);
}
