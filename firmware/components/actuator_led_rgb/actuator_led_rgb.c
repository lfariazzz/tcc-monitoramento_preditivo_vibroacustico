#include "actuator_led_rgb.h"
#include "driver/gpio.h"

#define LED_GPIO_R  42
#define LED_GPIO_G  45
#define LED_GPIO_B  46

// Catodo comum confirmado por teste físico: nível ALTO acende cada cor.
#define COR_ACESA   1
#define COR_APAGADA 0

esp_err_t actuator_led_rgb_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << LED_GPIO_R) | (1ULL << LED_GPIO_G) | (1ULL << LED_GPIO_B),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&cfg);
}

void actuator_led_rgb_set(led_severidade_t nivel)
{
    switch (nivel) {
        case LED_SEVERIDADE_NORMAL: // verde
            gpio_set_level(LED_GPIO_R, COR_APAGADA);
            gpio_set_level(LED_GPIO_G, COR_ACESA);
            gpio_set_level(LED_GPIO_B, COR_APAGADA);
            break;
        case LED_SEVERIDADE_LEVE: // amarelo = vermelho + verde
            gpio_set_level(LED_GPIO_R, COR_ACESA);
            gpio_set_level(LED_GPIO_G, COR_ACESA);
            gpio_set_level(LED_GPIO_B, COR_APAGADA);
            break;
        case LED_SEVERIDADE_SEVERA: // vermelho
            gpio_set_level(LED_GPIO_R, COR_ACESA);
            gpio_set_level(LED_GPIO_G, COR_APAGADA);
            gpio_set_level(LED_GPIO_B, COR_APAGADA);
            break;
    }
}