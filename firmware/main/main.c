#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define KY038_D0_GPIO  3

static volatile int contador_picos = 0;

static void IRAM_ATTR isr_ky038(void *arg)
{
    contador_picos++;
}

void app_main(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << KY038_D0_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,  // dispara na borda de subida
    };
    gpio_config(&cfg);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(KY038_D0_GPIO, isr_ky038, NULL);

    printf("Aguardando picos de som no D0...\n");

    for (;;) {
        printf("Picos detectados: %d\n", contador_picos);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}