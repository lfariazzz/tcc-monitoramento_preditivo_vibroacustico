#include "sensor_ky038.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define KY038_D0_GPIO  3

static volatile uint32_t s_contador_picos = 0;

static void IRAM_ATTR isr_ky038(void *arg)
{
    s_contador_picos++;
}

esp_err_t sensor_ky038_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << KY038_D0_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    esp_err_t err = gpio_config(&cfg);
    if (err != ESP_OK) return err;

    // gpio_install_isr_service pode já ter sido chamado por outro
    // componente — ESP_ERR_INVALID_STATE nesse caso é esperado, não é erro.
    err = gpio_install_isr_service(0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;

    return gpio_isr_handler_add(KY038_D0_GPIO, isr_ky038, NULL);
}

uint32_t sensor_ky038_pico_count_reset(void)
{
    portDISABLE_INTERRUPTS();
    uint32_t contador = s_contador_picos;
    s_contador_picos = 0;
    portENABLE_INTERRUPTS();
    return contador;
}