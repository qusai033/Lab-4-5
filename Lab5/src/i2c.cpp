#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR&(1<<TWINT)));

void initI2C() {
  // Wake up I2C module on mega 2560
  PRR0 &= ~(1 << PRTWI);

  // Set prescaler TWPS to 1
  TWSR &= ~(1 << TWPS0);
  TWSR &= ~(1 << TWPS1);

  // Set two wire interface bit rate register TWBR
  //TWBR = (CPU_clk_fq/SCL_fq - 16)/(2 * 4^TWPS)
  TWBR = 0xC6; //198

  // Enable two wire interface
  TWCR |= (1 << TWINT)|(1 << TWEN);
}


void StartI2C_Trans(unsigned char SLA) {
  // Clear TWINT, initiate start condition, initiate enable
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

  // Wait for completion
  while (!(TWCR & (1 << TWINT)));

  // Set two wire data register to the SLA + write bit
  TWDR = (SLA << 1);

  // Trigger action: Clear TWINT and initiate enable
  TWCR = (1 << TWINT) | (1 << TWEN);
  
  // Wait for completion
  while (!(TWCR & (1 << TWINT)));
}


void StopI2C_Trans(){
  //- Trigger action + stop condition
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}


void write(unsigned char data) {
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
    StartI2C_Trans(SLA);

    // Wait for completion
    write(MEMADDRESS);

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
