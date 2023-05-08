#ifndef RC522_H
#define RC522_H

#include <stdbool.h>
#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void reset();
void antennaOn();
void writeRegister(uint8_t reg, uint8_t value);
uint8_t readRegister(uint8_t reg);
void initRC522();
bool readTag(uint8_t *tag);
bool authenticateTag(const uint8_t *tag, const uint8_t *key);
void haltTag();

#endif // RC522_H
