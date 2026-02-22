#include <xc.h>
#include <stdint.h>

// CONFIG
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define FLASH_ADDR 0x7000

void FlashWriteOne(uint16_t address, uint8_t data);
uint8_t FlashReadByte(uint16_t address);

void flash1byte_main(void)
{
    uint8_t value;

    // Clock interno 8MHz
    OSCCON = 0x72;

    // PORTD como saída
    TRISD = 0x00;
    LATD = 0x00;

    // Escreve 0x55 na Flash
    FlashWriteOne(FLASH_ADDR, 0x55);

    // Lê o valor gravado
    value = FlashReadByte(FLASH_ADDR);

    // Mostra nos LEDs
    LATD = value;

    while(1);
}

void FlashWriteOne(uint16_t address, uint8_t data)
{
    // Alinha para bloco de 64 bytes
    address &= 0xFFC0;

    // --- ERASE ---
    TBLPTRU = 0;
    TBLPTRH = (address >> 8) & 0xFF;
    TBLPTRL = address & 0xFF;

    EECON1bits.EEPGD = 1;   // Program memory
    EECON1bits.CFGS = 0;    // Not config
    EECON1bits.FREE = 1;    // Erase
    EECON1bits.WREN = 1;    // Enable write

    INTCONbits.GIE = 0;     // Disable interrupts

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while(EECON1bits.WR);   // Wait finish

    EECON1bits.FREE = 0;    // Disable erase

    // --- WRITE ONE BYTE ---
    TBLPTRU = 0;
    TBLPTRH = (address >> 8) & 0xFF;
    TBLPTRL = address & 0xFF;

    TABLAT = data;
    asm("TBLWT*");

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while(EECON1bits.WR);

    INTCONbits.GIE = 1;     // Enable interrupts
    EECON1bits.WREN = 0;    // Disable write
}

uint8_t FlashReadByte(uint16_t address)
{
    TBLPTRU = 0;
    TBLPTRH = (address >> 8) & 0xFF;
    TBLPTRL = address & 0xFF;

    asm("TBLRD*");
    return TABLAT;
}