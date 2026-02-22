#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL   // 8 MHz interno

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

// =================================================

void blink_main(void)
{
    // Oscilador interno 8 MHz
    OSCCON = 0b01110010;

    // RD7 como saída
    TRISD = 0b01111111;

    // Inicializa apagado
    LATD = 0x00;

    while (1)
    {
        LATDbits.LATD7 = ~LATDbits.LATD7;  // toggle LED

        __delay_ms(200);  // ~200 ms visível
    }
}