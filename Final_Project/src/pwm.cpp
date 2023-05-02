#include <avr/io.h>
#include "pwm.h"
#include <Arduino.h>

#define arduino_clk_fq 16000000

void initPWMTimer3(){
    DDRE |= (1<<PORTE5); //output when using OCR3C for duty cycle mode 15 variable fq port 3
    //DDRH |= (1<<PORTH3); //set PIN 6 to output  
    //DDRH |= (1<<PORTH4); //set PIN 7 to output = 1A tocontrol motor

    //Timer 3
    //set non-inverting mode: output starts high and then is low
    TCCR3A |= (1<< COM3C1); //set output for OCR to be non-inverting
    TCCR3A &= ~(1<<COM3C0);

    //set WGM bits for PWM mode 7 fast 10 bit

    //Fast PWM for 10 bit top 
    //WGM33 = 0  WGM32 = 1 WGM31= 1 WGM30= 1
    //WGM33 and WGM32 are in TCCR3B
    //WGM31 and WGM30 are in TCCR3A

    TCCR3B &= ~(1<<WGM32) | (1<<WGM33);
    TCCR3A |= (1<<WGM31)|(1<<WGM30);

    /// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
    //TOP = 1023

    //prescalar of 256
    TCCR3B &= ~(1<<CS00) | (1<<CS01);
    TCCR3B |=  (1<<CS02);
}


void setLocked(bool locked){
    if(locked){
        OCR3C = 35;
    }
    else{
        OCR3C = 70;
    
    }
}
