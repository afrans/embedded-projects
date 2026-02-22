// This program write eeprom: 00 = 02 ; 01 = 04 ; 02 = 08 and when press the 
// button the leds show position 0,1,2,0 ....

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000

// ================= CONFIG =================
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config STVREN = OFF
// ===========================================

#define Switch_Pin PORTBbits.RB0


// ================= EEPROM =================
void EEPROM_Write(uint8_t address, uint8_t data)
{
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS  = 0;

    EEADR  = address;
    EEDATA = data;

    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;

    while(EECON1bits.WR);

    EECON1bits.WREN = 0;
}

uint8_t EEPROM_Read(uint8_t address)
{
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS  = 0;

    EEADR = address;
    EECON1bits.RD = 1;

    return EEDATA;
}
// ===========================================


void e2p_main(void)
{
    uint8_t current_index = 0;
    uint8_t SwitchPressed = 0;
    uint8_t value;

    // ===== Clock 8 MHz =====
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS  = 0b10;

    ANSEL  = 0x00;
    ANSELH = 0x00;

    // ===== LEDs =====
    TRISD = 0x00;
    LATD  = 0xFF;   // ativo LOW ? apagado

    // ===== Botão RB0 =====
    INTCON2bits.RBPU = 0;
    WPUBbits.WPUB0   = 1;
    TRISBbits.TRISB0 = 1;

    // =====================================================
    // ? Grava valores fixos no início
    // =====================================================
    EEPROM_Write(0x00, 0x02);
    EEPROM_Write(0x01, 0x04);
    EEPROM_Write(0x02, 0x08);

    // Pisca para indicar que gravou
    LATD = 0x00;
    __delay_ms(300);
    LATD = 0xFF;
    __delay_ms(300);

    // =====================================================
    // LOOP PRINCIPAL
    // =====================================================
    while(1)
    {
        if (Switch_Pin == 1)
        {
            SwitchPressed = 0;
        }
        else if (SwitchPressed == 0)
        {
            SwitchPressed = 1;

            // Lê valor atual
            value = EEPROM_Read(current_index);

            // Mostra nos LEDs (invertido)
            LATD = ~value;

            // Próxima posição
            current_index++;

            if (current_index > 2)
                current_index = 0;
        }
    }
}