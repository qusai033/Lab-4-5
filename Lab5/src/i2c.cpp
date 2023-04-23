#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR&(1<<TWINT)));

void initI2C(){

}

void StartI2C_Trans(unsigned char SLA){

}

void StopI2C_Trans(){
  //- Trigger action + stop condition
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void write(unsigned char data){

}
void Read_from(unsigned char SLA, unsigned char MEMADDRESS){

}

unsigned char Read_data(){
  return TWDR; //Returns the last byte
}
