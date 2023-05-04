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
    
    
    // Set SPI mode
    spiWriteRegister(0x01, 0x00);

    // Set Timer
    spiWriteRegister(0x2a, 0x8d);
    spiWriteRegister(0x2b, 0x3e);

    // Set Antenna Gain
    spiWriteRegister(0x0c, 0x00);

    // Turn on Antenna
    spiWriteRegister(0x14, 0x03);

    // Disable CRC
    spiWriteRegister(0x03, 0x80);

    // Clear FIFO buffer
    spiWriteRegister(0x0f, 0x80);
    spiWriteRegister(0x0f, 0x00);

    // Wait for the RC522 to finish initializing
    delayMs(100);
}

bool readTag(uint8_t *tag) {
    // Implement the read function for RC522 using SPI communication
    // ...(send the appropriate commands and read the tag data)
    // adjust this code according to RFID module's datasheet.
    
    uint8_t status;
    uint8_t buffer[18];
    uint8_t size = sizeof(buffer);

    // Send the command to read a tag
    status = spiTransfer(RC522_COMMAND_READ, NULL, 0, buffer, &size);

    // Check if a tag was successfully read
    if (status == 0 && size == 18) {
        // Copy the tag data to the provided buffer
        for (int i = 0; i < 16; i++) {
            tag[i] = buffer[i];
        }
        return true;
    } else {
        return false; // Return true if a tag is read successfully, otherwise return false
    }
}

bool authenticateTag(const uint8_t *tag, const uint8_t *key) {
    // Implement the authenticate function for RC522 using SPI communication
    // ...(send the authentication command and verify the response)
    // adjust this code according to RFID module's datasheet.
    
    uint8_t status;
    uint8_t buffer[2];
    uint8_t size = sizeof(buffer);

    // Send the command to authenticate the tag
    uint8_t authCommand[12] = {RC522_COMMAND_AUTH, 0x60, 0, 4};
    for (int i = 0; i < 4; i++) {
        authCommand[i+4] = tag[i];
        authCommand[i+8] = key[i];
    }
    status = spiTransferBlock(authCommand, sizeof(authCommand), buffer, &size);

    // Check if authentication was successful
    if (status == 0 && size == 2 && buffer[0] == 0x00 && buffer[1] == 0x00) {
        return true;
    } else {
        return false;// Return true if authentication is successful, otherwise return false
    }
}

void haltTag() {
    // Implement the halt function for RC522 using SPI communication
    // ...(send the halt command to stop communication with the tag)
    // adjust this code according to RFID module's datasheet.
    
    
    // Send the command to halt communication with the tag
    spiTransfer(RC522_COMMAND_HALT, NULL, 0, NULL, NULL);
}
