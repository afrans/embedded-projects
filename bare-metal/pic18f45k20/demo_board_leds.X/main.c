#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL   // 8 MHz

// ================= CONFIG BITS =================

// CONFIG1H
#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF

// CONFIG2L
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30

// CONFIG2H
#pragma config WDTEN = OFF, WDTPS = 32768

// CONFIG3H
#pragma config MCLRE = OFF, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC

// CONFIG4L
#pragma config STVREN = ON, LVP = OFF, XINST = OFF

// ================================================

// Lookup table (XC8 ? const normal)
const uint8_t LED_LookupTable[8] =
{
    0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80
};

void main(void)
{
    uint8_t LED_Number = 0;

    // Oscilador interno 8 MHz
    OSCCON = 0b01110010;

    TRISD = 0x00;   // PORTD como saída
    LATD  = 0x00;

    while (1)
    {
        LATD = LED_LookupTable[LED_Number];

        LED_Number++;
        if (LED_Number >= 8)
            LED_Number = 0;

        __delay_ms(200);   // pisca visível
    }
}
