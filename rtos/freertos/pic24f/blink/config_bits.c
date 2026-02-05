#include <xc.h>

/* =========================================================
 * Configuration bits for PIC24FJ128GA306
 * ========================================================= */

/* Oscillator */
#pragma config FNOSC = FRC
#pragma config IESO  = OFF

/* Clock switching / Fail-Safe Clock Monitor */
#pragma config FCKSM = CSECMD

/* Debug / JTAG */
#pragma config JTAGEN = OFF
