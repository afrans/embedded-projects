#include <xc.h>

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 8000000UL

void pwm_main(void)
{
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS  = 0b10;

    ANSEL = 0;
    ANSELH = 0;

    TRISDbits.TRISD7 = 0;
    
    CCPR1L = 10;   // bem fraco

    while(1)
    {
        LATDbits.LATD7 = 0;
        __delay_ms(500);
        LATDbits.LATD7 = 1;
        __delay_ms(500);
    }
}