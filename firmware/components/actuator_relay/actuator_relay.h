#pragma once
#include "esp_err.h"

/** Configura o GPIO de controle do transistor (base) como saída. */
esp_err_t actuator_relay_init(void);

/** Energiza o circuito (transistor conduz — ventilador ligado). */
void actuator_relay_on(void);

/** Corta a energia (transistor corta — ventilador desligado). RF-08. */
void actuator_relay_off(void);