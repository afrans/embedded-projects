#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL   // 8 MHz

// ================= CONFIG BITS =================

#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOREN = SBORDIS, BORV = 30
#pragma config WDTEN = OFF, WDTPS = 32768
#pragma config MCLRE = OFF, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC
#pragma config STVREN = ON, LVP = OFF, XINST = OFF

// =================================================

// Macro da chave (RB0)
#define Switch_Pin PORTBbits.RB0
#define DetectsInARow 5

void switch_main(void)
{
    uint8_t LED_Display = 1;
    uint8_t Switch_Count = 0;

    // Oscilador interno 8 MHz
    OSCCON = 0b01110010;

    // PORTD saída (LEDs)
    TRISD = 0x00;
    LATD = 0x00;

    // Configuração da chave RB0
    INTCON2bits.RBPU = 0;   // habilita pull-ups internos
    WPUBbits.WPUB0 = 1;     // pull-up em RB0
    ANSELH = 0x00;          // desativa analógico
    TRISBbits.TRISB0 = 1;   // RB0 como entrada

    while (1)
    {
        LATD = LED_Display;

        // Próximo LED (rotaciona)
        LED_Display <<= 1;
        if (LED_Display == 0)
            LED_Display = 1;

        // Espera soltar o botão
        while (Switch_Pin != 1);

        // Debounce
        Switch_Count = 0;
        do
        {
            if (Switch_Pin == 0)
                Switch_Count++;
            else
                Switch_Count = 0;

            __delay_ms(1);

        } while (Switch_Count < DetectsInARow);
    }
}