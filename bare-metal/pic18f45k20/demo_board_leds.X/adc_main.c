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

#define Switch_Pin PORTBbits.RB0

typedef enum
{
    LEFT2RIGHT = 0,
    RIGHT2LEFT
} LEDDirections;

// ================= ADC =====================
void ADC_Init(void)
{
    ANSEL  = 0x01;   // RA0 analógico
    ANSELH = 0x00;

    ADCON1 = 0x00;   // Vref = VDD/VSS
    ADCON2 = 0b10101010; 
    // ADFM=1 (10 bits)
    // ACQT=12TAD
    // ADCS=FOSC/32 (melhor para 8MHz)

    ADCON0 = 0b00000001;  // Canal AN0 + ADC ON
}

uint16_t ADC_Read(void)
{
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);
return ((uint16_t)ADRESH << 8) | ADRESL;}

// ================= MAIN ====================
void adc_main(void)
{
    uint8_t LED_Display = 1;
    LEDDirections Direction = LEFT2RIGHT;
    uint8_t SwitchPressed = 0;
    uint8_t tick = 0;
    uint16_t adc_value;
    uint8_t speed;

    // ===== Clock 8 MHz =====
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS  = 0b10;

    // ===== LEDs =====
    TRISD = 0x00;
    LATD  = LED_Display;

    // ===== Botão RB0 =====
    INTCON2bits.RBPU = 0;
    WPUBbits.WPUB0   = 1;
    ANSELH = 0x00;
    TRISBbits.TRISB0 = 1;

    // ===== ADC =====
    ADC_Init();

    // ===== TIMER0 =====
    T0CON = 0;
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS   = 0;
    T0CONbits.PSA    = 0;
    T0CONbits.T0PS   = 7;   // 1:256
    T0CONbits.TMR0ON = 1;

    INTCONbits.TMR0IF = 0;

    while (1)
    {
        if (INTCONbits.TMR0IF)
        {
            INTCONbits.TMR0IF = 0;

            tick++;

            // Lê potenciômetro
            adc_value = ADC_Read();

            // Converte 0?1023 para 5?50 (velocidade segura)
speed = (uint8_t)((adc_value >> 4) + 5);
            if (tick >= speed)
            {
                tick = 0;

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

                LATD = LED_Display;
            }
        }

        // ===== Botão =====
        if (Switch_Pin == 1)
        {
            SwitchPressed = 0;
        }
        else if (SwitchPressed == 0)
        {
            SwitchPressed = 1;
            Direction = (Direction == LEFT2RIGHT) ?
                        RIGHT2LEFT : LEFT2RIGHT;
        }
    }
}