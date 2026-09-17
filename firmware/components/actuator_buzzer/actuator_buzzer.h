#pragma once
#include "esp_err.h"

/** Configura o GPIO do buzzer como saída. */
esp_err_t actuator_buzzer_init(void);

/** Liga o buzzer (nível alto). */
void actuator_buzzer_on(void);

/** Desliga o buzzer (nível baixo). */
void actuator_buzzer_off(void);