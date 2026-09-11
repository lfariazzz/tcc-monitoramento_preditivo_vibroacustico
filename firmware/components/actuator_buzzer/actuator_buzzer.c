#include "actuator_buzzer.h"
#include "driver/gpio.h"

#define BUZZER_GPIO  47

esp_err_t actuator_buzzer_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << BUZZER_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&cfg);
}

void actuator_buzzer_on(void)
{
    gpio_set_level(BUZZER_GPIO, 1);
}

void actuator_buzzer_off(void)
{
    gpio_set_level(BUZZER_GPIO, 0);
}