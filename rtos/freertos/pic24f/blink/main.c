#include <xc.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* LED no RG9 */
#define LED_RG9_TRIS  TRISGbits.TRISG9
#define LED_RG9       LATGbits.LATG9

/* Task */
static void vBlinkTask(void *pvParameters);

int main(void)
{
    /* Configura LED */
    LED_RG9_TRIS = 0;   // saída
    LED_RG9 = 0;        // desligado

    /* Cria task */
    xTaskCreate(
        vBlinkTask,
        "blink",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
    );

    /* Inicia RTOS */
    vTaskStartScheduler();

    /* Nunca chega aqui */
    while (1);
}

/* Pisca LED */
static void vBlinkTask(void *pvParameters)
{
    (void) pvParameters;

    for (;;)
    {
        LED_RG9 ^= 1;                 // toggle
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
