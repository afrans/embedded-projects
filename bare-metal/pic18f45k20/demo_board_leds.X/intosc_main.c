#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL

// ================= CONFIG =================
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config STVREN = OFF
// ===========================================

#define Switch_Pin PORTBbits.RB0

typedef enum
{
    FREQ_1MHZ = 0,
    FREQ_8MHZ,
    FREQ_64MHZ
} ClockMode;

ClockMode CurrentClock = FREQ_1MHZ;

void SetClock(void);

void intosc_main(void)
{
    uint8_t LED_Display = 0;

    // Desativa analógico
    ANSEL  = 0x00;
    ANSELH = 0x00;

    // LEDs
    TRISD = 0x00;
    LATD  = 0x00;

    // Botão RB0
    INTCON2bits.RBPU = 0;
    WPUBbits.WPUB0   = 1;
    TRISBbits.TRISB0 = 1;

    SetClock();

    while (1)
    {
        LATD = LED_Display++;

        // Delay fixo (a diferença será causada pelo clock)
        for(uint32_t i=0; i < 30000UL; i++);

        if(Switch_Pin == 0)
        {
            __delay_ms(50);

            if(Switch_Pin == 0)
            {
                if(CurrentClock == FREQ_64MHZ)
                    CurrentClock = FREQ_1MHZ;
                else
                    CurrentClock++;

                SetClock();

                while(Switch_Pin == 0);
            }
        }
    }
}

void SetClock(void)
{
    switch(CurrentClock)
    {
        case FREQ_1MHZ:
            OSCCONbits.IRCF = 0b011;   // 1 MHz
            OSCTUNEbits.PLLEN = 0;
            break;

        case FREQ_8MHZ:
            OSCCONbits.IRCF = 0b110;   // 8 MHz
            OSCTUNEbits.PLLEN = 0;
            break;

        case FREQ_64MHZ:
            OSCCONbits.IRCF = 0b111;   // 16 MHz
            OSCTUNEbits.PLLEN = 1;     // 16 x 4 = 64 MHz
            break;
    }

    OSCCONbits.SCS = 0b10; // força usar clock interno
}