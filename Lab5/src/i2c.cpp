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


void Write(unsigned char data) {
  // Set two wire data register equal to incoming data
  TWDR = data;

  // Trigger action
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Wait for completion
  //wait_for_completion;
  while (!(TWCR & (1 << TWINT)));
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
    // Start a transmission to the SLA
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // Wait for completion
    while (!(TWCR & (1 << TWINT)));

    // Write to the MEMADDRESS
    TWDR = MEMADDRESS;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for completion
    while (!(TWCR & (1 << TWINT)));

    // Clear TWINT, initiate start condition, initiate enable
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // Wait for completion
    //wait_for_completion;
    while (!(TWCR & (1 << TWINT)));

    // Set two wire data register to the SLA + read bit
    TWDR = (SLA << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    // Wait for completion
    while (!(TWCR & (1 << TWINT)));

    // Trigger action
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for completion
    while (!(TWCR & (1 << TWINT)));

    // Trigger action + stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}


unsigned char Read_data(){
  return TWDR; //Returns the last byte
}
