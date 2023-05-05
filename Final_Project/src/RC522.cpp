#include "RC522.h"


// Define commands for reading, authenticating, and halting the tag
#define RC522_COMMAND_READ 0x30
#define RC522_COMMAND_AUTH 0x60
#define RC522_COMMAND_HALT 0x50

// Function to reset the RC522
void reset() {
    writeRegister(MFRC522_COMMAND_REG, MFRC522_CMD_SOFT_RESET);
    _delay_ms(50);
    while (readRegister(MFRC522_COMMAND_REG) & (1 << 4)) {}
}

// Function to turn on the antenna of the RC522
void antennaOn() {
    uint8_t value = readRegister(MFRC522_TX_CONTROL_REG);
    if ((value & 0x03) != 0x03) {
        writeRegister(MFRC522_TX_CONTROL_REG, value | 0x03);
    }
}

// Function to write a register of the RC522
void writeRegister(uint8_t reg, uint8_t value) {
    PORTB &= ~(1 << RC522_SS_PIN); // Set the chip select pin low
    SPI.transfer(reg & 0x7F); // Send the register address
    SPI.transfer(value); // Send the value
    PORTB |= (1 << RC522_SS_PIN); // Set the chip select pin high
}

// Function to read a register of the RC522
uint8_t readRegister(uint8_t reg) {
    uint8_t data;
    PORTB &= ~(1 << RC522_SS_PIN); // Set the chip select pin low
    SPI.transfer(reg & 0x7F); // Send the register address
    data = SPI.transfer(0x00); // Send a dummy byte to receive data
    PORTB |= (1 << RC522_SS_PIN); // Set the chip select pin high
    return data;
}

// Function to initialize the RC522
void initRC522() {
    // Initialize SPI
    // Set MOSI, SCK, and SS as output
    DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
    // Set MISO as input
    DDRB &= ~(1 << DDB3);

    // Enable SPI, set as master, and set clock rate
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

        // Set reset pin as output
    DDRB |= (1 << DDE3);

    // Reset RC522
    PORTB &= ~(1 << RC522_RST_PIN);
    _delay_ms(10);
    PORTB |= (1 << RC522_RST_PIN);
    _delay_ms(10);

    // Additional initialization
    PORTB |= (1 << RC522_SS_PIN); // Set the chip select pin high
    PORTB |= (1 << RC522_RST_PIN); // Set the reset pin high

    reset();

    writeRegister(MFRC522_T_MODE_REG, 0x8D);
    writeRegister(MFRC522_T_PRESCALER_REG, 0x3E);
    writeRegister(MFRC522_T_RELOAD_REGL, 30);
    writeRegister(MFRC522_T_RELOAD_REGH, 0);

    writeRegister(MFRC522_TX_AUTO_REG, 0x40);
    writeRegister(MFRC522_MODE_REG, 0x3D);

    antennaOn();
}

// Function to read the RFID tag
bool readTag(uint8_t *tag) {
    // This function assumes the tag has a length of 3 bytes (40 bits)
    uint8_t tagLength = 3;
    
    // Select the RC522 by setting SS low
    PORTB &= ~(1 << RC522_SS_PIN);

    // Send the command to read the tag
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

// Function to authenticate the RFID tag
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

// Function to halt the RFID tag
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
