#include "RC522.h"
#include <avr/io.h>
#include <util/delay.h>

#define RC522_SS_PIN PORTB2
#define RC522_RST_PIN PORTB1

#define RC522_COMMAND_READ 0x30
#define RC522_COMMAND_AUTH 0x60
#define RC522_COMMAND_HALT 0x50

void initRC522() {
    // Initialize SPI
    // Set MOSI, SCK and SS as output
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
    // Set MISO as input
    DDRB &= ~(1 << DDB4);

    // Enable SPI, set as master, and set clock rate
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    // Set reset pin as output
    DDRB |= (1 << DDB1);

    // Reset RC522
    PORTB &= ~(1 << RC522_RST_PIN);
    delayMs(10);
    PORTB |= (1 << RC522_RST_PIN);
    delayMs(10);

    // Initialization code for RC522
    // Set SS pin as output
    DDRB |= (1 << RC522_SS_PIN);
    
    

    // Wait for the RC522 to finish initializing
    delayMs(100);
}

bool readTag(uint8_t *tag) {
    // This function assumes the tag has a length of 5 bytes (40 bits)
    uint8_t tagLength = 5;
    
    // Select the RC522 by setting SS low
    PORTB &= ~(1 << RC522_SS_PIN);

    // Send the command to read the tag
    // You may need to adjust this according to your RFID module's datasheet
    SPDR = RC522_COMMAND_READ;
    while (!(SPSR & (1 << SPIF)));
    
    // Send the address you want to read, for example, block 2
    SPDR = 2;
    while (!(SPSR & (1 << SPIF)));

    // Read the tag data
    for (uint8_t i = 0; i < tagLength; i++) {
        SPDR = 0; // Send dummy byte to receive data
        while (!(SPSR & (1 << SPIF)));
        tag[i] = SPDR;
    }

    // Deselect the RC522 by setting SS high
    PORTB |= (1 << RC522_SS_PIN);

    // Check if the tag was read successfully
    if (tag[0] != 0 && tag[1] != 0 && tag[2] != 0 && tag[3] != 0 && tag[4] != 0) {
        return true;
    } else {
        return false;
    }
}

bool authenticateTag(const uint8_t *tag, const uint8_t *key) {
    uint8_t status;
    uint8_t keyLength = 6; // The key length for MIFARE Classic is 6 bytes
    uint8_t blockAddress = 2; // The block you want to authenticate (change as needed)

    // Select the RC522 by setting SS low
    PORTB &= ~(1 << RC522_SS_PIN);

    // Send the command to authenticate the tag
    SPDR = RC522_COMMAND_AUTH;
    while (!(SPSR & (1 << SPIF)));

    // Send the authentication mode (e.g., 0x60 for MIFARE Classic key A)
    SPDR = 0x60;
    while (!(SPSR & (1 << SPIF)));

    // Send the block address
    SPDR = blockAddress;
    while (!(SPSR & (1 << SPIF)));

    // Send the key
    for (uint8_t i = 0; i < keyLength; i++) {
        SPDR = key[i];
        while (!(SPSR & (1 << SPIF)));
    }

    // Send the tag (UID)
    for (uint8_t i = 0; i < 4; i++) {
        SPDR = tag[i];
        while (!(SPSR & (1 << SPIF)));
    }

    // Read the status
    SPDR = 0; // Send dummy byte to receive status
    while (!(SPSR & (1 << SPIF)));
    status = SPDR;

    // Deselect the RC522 by setting SS high
    PORTB |= (1 << RC522_SS_PIN);

    // Check if the tag was authenticated successfully
    if (status == 0) {
        return true;
    } else {
        return false;
    }
}


void haltTag() {
    // Select the RC522 by setting SS low
    PORTB &= ~(1 << RC522_SS_PIN);

    // Send the command to halt the tag
    SPDR = RC522_COMMAND_HALT;
    while (!(SPSR & (1 << SPIF)));

    // Send the CRC bytes
    SPDR = 0x93; // CRC byte 1
    while (!(SPSR & (1 << SPIF)));

    SPDR = 0x70; // CRC byte 2
    while (!(SPSR & (1 << SPIF)));

    // Deselect the RC522 by setting SS high
    PORTB |= (1 << RC522_SS_PIN);
}

