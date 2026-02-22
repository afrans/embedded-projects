#include <xc.h>

#define RUN_HELLO
// #define RUN_BLINK
// #define RUN_ROTATE
// #define RUN_SWITCH
// #define RUN_TIMER
// #define RUN_ADC
// #define RUN_INTERRUPTS
// #define RUN_INTOSC
// #define RUN_EEPROM
// #define RUN_PROGRAM_MEM
// #define RUN_PWM

void hello_main(void);

void main(void)
{
#ifdef RUN_HELLO
    hello_main();
#endif

#ifdef RUN_BLINK
    blink_main();
#endif
    
#ifdef RUN_ROTATE
    rotate_main();
#endif
    
#ifdef RUN_SWITCH
    switch_main();
#endif
    
#ifdef RUN_TIMER
    timer_main();
#endif
    
#ifdef RUN_ADC
    adc_main();
#endif
    
#ifdef RUN_INTERRUPTS
    interrupts_main();
#endif
    
#ifdef RUN_INTOSC
    intosc_main();
#endif
    
#ifdef RUN_EEPROM
    e2p_main();
#endif
    
#ifdef RUN_PROGRAM_MEM_64
    flash64byte_main();
#endif
    
#ifdef RUN_PROGRAM_MEM_1
    flash1byte_main();
#endif
    
#ifdef RUN_pwm
    pwm_main();
#endif

}