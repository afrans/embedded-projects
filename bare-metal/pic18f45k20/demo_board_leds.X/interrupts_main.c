#include <xc.h>
#include <stdint.h>

// ================= CONFIG =================
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config STVREN = OFF
// ===========================================


// ================= TIPOS ===================
typedef enum {
    LEFT2RIGHT = 0,
    RIGHT2LEFT
} LEDDirections;


// ================= VARIÁVEIS GLOBAIS ===================
volatile uint8_t LED_Display = 1;
volatile LEDDirections Direction = LEFT2RIGHT;


// ================= PROTÓTIPOS ===================
void ADC_Init(void);
uint8_t ADC_Convert(void);
void Timer0_Init(void);


// ================= INTERRUPÇÃO ALTA ===================
void __interrupt(high_priority) HighISR(void)
{
    if (INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;

        if (Direction == LEFT2RIGHT)
            Direction = RIGHT2LEFT;
        else
            Direction = LEFT2RIGHT;
    }
}


// ================= INTERRUPÇÃO BAIXA ===================
void __interrupt(low_priority) LowISR(void)
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        // Recarrega timer com valor do ADC
        TMR0L = 0;
        TMR0H = ADC_Convert();

        if (Direction == LEFT2RIGHT)
        {
            LED_Display <<= 1;
            if (LED_Display == 0)
                LED_Display = 1;
        }
        else
        {
            LED_Display >>= 1;
            if (LED_Display == 0)
                LED_Display = 0x80;
        }
    }
}


// ================= MAIN ===================
void interrupts_main(void)
{
    // ===== Clock interno 8 MHz =====
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS  = 0b10;

    // ===== LEDs =====
    TRISD = 0x00;
    LATD  = LED_Display;

    // ===== Botão INT0 (RB0) =====
    INTCON2bits.RBPU = 0;      // habilita pull-ups
    WPUBbits.WPUB0   = 1;      // pull-up RB0
    ANSELH = 0x00;             // PORTB digital
    TRISBbits.TRISB0 = 1;

    INTCON2bits.INTEDG0 = 0;   // borda de descida
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;

    // ===== Inicializações =====
    ADC_Init();
    Timer0_Init();

    // ===== Habilita prioridades =====
    RCONbits.IPEN = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.GIEH = 1;

    while (1)
    {
        LATD = LED_Display;
    }
}


// ================= TIMER0 ===================
void Timer0_Init(void)
{
    T0CON = 0;

    T0CONbits.T08BIT = 0;   // 16-bit
    T0CONbits.T0CS   = 0;   // clock interno
    T0CONbits.PSA    = 0;   // prescaler ON
    T0CONbits.T0PS   = 7;   // 1:256

    TMR0H = 0;
    TMR0L = 0;

    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 0;  // baixa prioridade
    INTCONbits.TMR0IE = 1;

    T0CONbits.TMR0ON = 1;
}


// ================= ADC ===================
void ADC_Init(void)
{
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1;     // RA0 analógico

    ADCON2 = 0b00111000;
    ADCON0 = 0b00000001;
}

uint8_t ADC_Convert(void)
{
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);

    return ADRESH;
}