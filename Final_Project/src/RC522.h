#ifndef RC522_H
#define RC522_H

#include <avr/io.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

// SPI communication functions
void initSPI();
uint8_t spiTransfer(uint8_t data);
uint8_t spiTransferBlock(const uint8_t *dataOut, uint8_t dataOutSize, uint8_t *dataIn, uint8_t *dataInSize);

// RC522 functions
void initRC522();
bool readTag(uint8_t *tag);
bool authenticateTag(const uint8_t *tag, const uint8_t *key);
void haltTag();

#endif // RC522_H
