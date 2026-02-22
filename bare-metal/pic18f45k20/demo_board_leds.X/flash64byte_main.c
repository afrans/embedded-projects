// Show 0xA0 in leds thst is position 0x7000

#include <xc.h>
#include <stdint.h>

// CONFIG BITS
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define FLASH_ADDR 0x7000

void FlashErase64(uint16_t address);
void FlashWrite32(uint16_t address, uint8_t *data);
uint8_t FlashReadByte(uint16_t address);

void flash64byte_main(void)
{
    uint8_t buffer[32];
    uint8_t i;
    uint8_t value;

    // Clock interno 8MHz
    OSCCON = 0x72;

    // PORTD como saída (LEDs)
    TRISD = 0x00;
    LATD = 0x00;

    // Preenche buffer com padrão 0xA0..0xBF
    for(i = 0; i < 32; i++)
    {
        buffer[i] = 0xA0 + i;
    }

    FlashErase64(FLASH_ADDR);
    FlashWrite32(FLASH_ADDR, buffer);

    value = FlashReadByte(FLASH_ADDR);

    LATD = value;   // Mostra nos LEDs

    while(1);
}

void FlashErase64(uint16_t address)
{
    address &= 0xFFC0; // alinhamento 64 bytes

    TBLPTRU = 0;
    TBLPTRH = (uint8_t)(address >> 8);
    TBLPTRL = (uint8_t)(address & 0xFF);

    EECON1bits.EEPGD = 1;  // Program memory
    EECON1bits.CFGS = 0;
    EECON1bits.FREE = 1;   // Erase
    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while(EECON1bits.WR);  // Espera terminar

    INTCONbits.GIE = 1;
    EECON1bits.WREN = 0;
}

void FlashWrite32(uint16_t address, uint8_t *data)
{
    uint8_t i;

    address &= 0xFFE0; // alinhamento 32 bytes

    TBLPTRU = 0;
    TBLPTRH = (uint8_t)(address >> 8);
    TBLPTRL = (uint8_t)(address & 0xFF);

    // Carrega 31 bytes com incremento
    for(i = 0; i < 31; i++)
    {
        TABLAT = data[i];
        asm("TBLWT*+");
    }

    // Último byte sem incrementar
    TABLAT = data[31];
    asm("TBLWT*");

    EECON1bits.EEPGD = 1;
    EECON1bits.CFGS = 0;
    EECON1bits.FREE = 0;  // Write mode
    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while(EECON1bits.WR);

    INTCONbits.GIE = 1;
    EECON1bits.WREN = 0;
}

uint8_t FlashReadByte(uint16_t address)
{
    TBLPTRU = 0;
    TBLPTRH = (uint8_t)(address >> 8);
    TBLPTRL = (uint8_t)(address & 0xFF);

    asm("TBLRD*");
    return TABLAT;
}