/*
 * File:   main.c
 * Author: ander
 *
 * Created on February 20, 2026, 6:42 PM
 */

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL   // 8 MHz interno

// ================= CONFIG BITS =================

// CONFIG1H
#pragma config FOSC = INTIO67
#pragma config FCMEN = OFF
#pragma config IESO = OFF

// CONFIG2H
#pragma config WDTEN = OFF

// CONFIG3H
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#pragma config PBADEN = OFF

// ===============================================

void hello_main(void)
{
    // Configura oscilador interno para 8 MHz
    OSCCON = 0b01110010;

    // RD7 como saída, resto entrada
    TRISD = 0b01111111;

    // Garante que começa desligado
    LATD = 0x00;

    // Liga LED 7
    LATDbits.LATD7 = 1;

    while(1)
    {
        // Loop infinito
    }
}
