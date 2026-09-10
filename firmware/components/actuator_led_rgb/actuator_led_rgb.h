#pragma once
#include "esp_err.h"

typedef enum {
    LED_SEVERIDADE_NORMAL = 0,  // verde
    LED_SEVERIDADE_LEVE,        // amarelo (vermelho + verde)
    LED_SEVERIDADE_SEVERA,      // vermelho
} led_severidade_t;

/** Configura os 3 GPIOs do LED RGB como saída. */
esp_err_t actuator_led_rgb_init(void);

/** Ajusta a cor do LED conforme o nível de severidade. */
void actuator_led_rgb_set(led_severidade_t nivel);