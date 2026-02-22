#include <xc.h>

// ================= CONFIG =================
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config STVREN = OFF
#pragma config PBADEN = OFF
// ===========================================

#define _XTAL_FREQ 8000000UL   // 8 MHz (igual seu código que funcionou)

void timer_main(void)
{
    unsigned char brightness = 0;

    // ===== Clock 8 MHz =====
    OSCCONbits.IRCF = 0b111;   // 8 MHz
    OSCCONbits.SCS  = 0b10;    // clock interno

    // ===== Tudo digital =====
    ANSEL  = 0x00;
    ANSELH = 0x00;

    // ===== RD7 saída (P1D) =====
    TRISDbits.TRISD7 = 0;

    // ===== Timer2 para PWM =====
    T2CON = 0b00000111;  // Prescaler 1:16, Timer2 ON
    PR2   = 124;         // Ajustado para ~1kHz com 8MHz

    // ===== PWM modo Full Bridge para usar P1D =====
    CCP1CON = 0b01001100;   // PWM em P1D (RD7)

    CCPR1L = 0;   // começa apagado

    while(1)
    {
        // Aumenta brilho
        for(brightness = 0; brightness < 255; brightness++)
        {
            CCPR1L = 255 - brightness;  // invertido (LED ativo baixo)
            __delay_ms(5);
        }

        // Diminui brilho
        for(brightness = 255; brightness > 0; brightness--)
        {
            CCPR1L = 255 - brightness;  // invertido
            __delay_ms(5);
        }
    }
}