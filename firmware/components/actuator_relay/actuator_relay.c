#include "actuator_relay.h"
#include "driver/gpio.h"

// Base do transistor 2N2222 — chaveia o circuito de 5V do ventilador
// (coletor/emissor), não o próprio motor diretamente.
#define RELAY_GPIO  2

esp_err_t actuator_relay_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << RELAY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&cfg);
}

void actuator_relay_on(void)
{
    gpio_set_level(RELAY_GPIO, 1);
}

void actuator_relay_off(void)
{
    gpio_set_level(RELAY_GPIO, 0);
}