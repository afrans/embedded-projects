#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO 2

void app_main(void)
{
    printf("ESP32-S3 rodando OK1!\n");

    // Configura GPIO 2 como saída
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_GPIO, 1);
        printf("Led ON\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_GPIO, 0);
        printf("Led OFF\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}