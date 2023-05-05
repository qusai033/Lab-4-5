#ifndef RC522_H
#define RC522_H

#include <stdbool.h>
#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>

// Define the pin assignments for RC522
#define RC522_SS_PIN PORTB0
#define RC522_RST_PIN PORTE3

// Define commands for reading, authenticating, and halting the tag
#define RC522_COMMAND_READ 0x30
#define RC522_COMMAND_AUTH 0x60
#define RC522_COMMAND_HALT 0x50

// Define missing constants
#define MFRC522_COMMAND_REG 0x01
#define MFRC522_CMD_SOFT_RESET 0x0F
#define MFRC522_TX_CONTROL_REG 0x14
#define MFRC522_T_MODE_REG 0x2A
#define MFRC522_T_PRESCALER_REG 0x2B
#define MFRC522_T_RELOAD_REGL 0x2C
#define MFRC522_T_RELOAD_REGH 0x2D
#define MFRC522_TX_AUTO_REG 0x15
#define MFRC522_MODE_REG 0x11

// Function prototypes
void reset();
void antennaOn();
void writeRegister(uint8_t reg, uint8_t value);
uint8_t readRegister(uint8_t reg);
void initRC522();
bool readTag(uint8_t *tag);
bool compareTags(const uint8_t *tag1, const uint8_t *tag2, uint8_t length);
bool authenticateTag(const uint8_t *tag, const uint8_t *key);
void haltTag();

#endif // RC522_H
