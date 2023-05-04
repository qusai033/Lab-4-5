#ifndef RC522_H
#define RC522_H

// Define the necessary data types
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

// Declare the functions used for RC522
void initRC522();
bool readTag(uint8_t *tag);
bool authenticateTag(const uint8_t *tag, const uint8_t *key);
void haltTag();

// Declare the helper functions used for SPI communication
void spiTransfer(uint8_t *sendData, uint8_t *receiveData, uint8_t length);
void spiWriteRegister(uint8_t reg, uint8_t data);
void delayMs(uint16_t delay);

#endif
