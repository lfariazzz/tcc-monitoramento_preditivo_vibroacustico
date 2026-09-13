#pragma once
#include "esp_err.h"
#include <stdint.h>

/**
 * Inicializa o GPIO de interrupção do KY-038 (D0) — detecção de pico
 * sonoro via limiar de hardware (RF-03).
 *
 * NOTA: o A0 (sinal fino, via ADS1115) ainda não está implementado neste
 * componente — ver Issue 8, pendente de fiação do ADS1115.
 */
esp_err_t sensor_ky038_init(void);

/**
 * Retorna quantos picos foram detectados desde a última chamada, e zera
 * o contador (leitura destrutiva) — pensado para ser consumido uma vez
 * por janela de aquisição.
 */
uint32_t sensor_ky038_pico_count_reset(void);