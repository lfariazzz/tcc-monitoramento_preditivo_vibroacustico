#pragma once
#include "esp_err.h"
#include <stdint.h>

/**
 * Inicializa o KY-038 por completo: GPIO de interrupção do D0 (pico
 * sonoro, RF-03) e o ADS1115 no canal A0 (sinal fino, RF-02).
 */
esp_err_t sensor_ky038_init(void);

/**
 * Retorna quantos picos foram detectados desde a última chamada, e zera
 * o contador (leitura destrutiva) — pensado para ser consumido uma vez
 * por janela de aquisição.
 */
uint32_t sensor_ky038_pico_count_reset(void);

/**
 * Lê o valor bruto atual do sinal fino de som (A0, via ADS1115).
 * Repouso calibrado em bancada na faixa de ~10000 (ajustado via trimpot
 * físico do módulo). Não é uma unidade física convertida — é o valor
 * bruto de 16 bits do ADC, mesma lógica de uso do BNO085 (o modelo de IA
 * extrai características em cima do valor bruto, não de uma conversão
 * para dB).
 */
esp_err_t sensor_ky038_ler_som(int16_t *valor_bruto);