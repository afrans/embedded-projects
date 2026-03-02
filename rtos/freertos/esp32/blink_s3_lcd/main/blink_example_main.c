#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

// ===== LCD PINS =====
#define RS 4
#define E  5
#define D4 15
#define D5 16
#define D6 17
#define D7 18

// ===== LED =====
#define LED_GPIO 7

// ===== LCD FUNCTIONS =====

static void lcd_pulse_enable() {
    gpio_set_level(E, 1);
    esp_rom_delay_us(1);
    gpio_set_level(E, 0);
    esp_rom_delay_us(100);
}

static void lcd_send_nibble(uint8_t nibble) {
    gpio_set_level(D4, (nibble >> 0) & 1);
    gpio_set_level(D5, (nibble >> 1) & 1);
    gpio_set_level(D6, (nibble >> 2) & 1);
    gpio_set_level(D7, (nibble >> 3) & 1);
    lcd_pulse_enable();
}

static void lcd_send_byte(uint8_t data, uint8_t is_data) {
    gpio_set_level(RS, is_data);
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
}

static void lcd_data(uint8_t data) {
    lcd_send_byte(data, 1);
}

static void lcd_init(void)
{
    gpio_set_direction(RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(E, GPIO_MODE_OUTPUT);
    gpio_set_direction(D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(D7, GPIO_MODE_OUTPUT);

    gpio_set_level(RS, 0);
    gpio_set_level(E, 0);

    vTaskDelay(pdMS_TO_TICKS(100));  // espera bem mais no power

    // força modo 8 bits 3 vezes
    for(int i = 0; i < 3; i++) {
        lcd_send_nibble(0x03);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // entra em 4 bits
    lcd_send_nibble(0x02);
    vTaskDelay(pdMS_TO_TICKS(10));

    lcd_cmd(0x28);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x08);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(10));  // clear precisa mais tempo

    lcd_cmd(0x06);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x0C);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x80);  // força cursor
    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_print(char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

void app_main(void)
{
    printf("ESP32-S3 rodando OK!\n");

    // Configura LED
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Inicializa LCD
    lcd_init();
    lcd_print("ESP32-S3 OK");

    while (1)
    {
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}