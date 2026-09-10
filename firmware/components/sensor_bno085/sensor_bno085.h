#pragma once
#include "esp_err.h"

/** Uma leitura do acelerômetro calibrado (m/s², com gravidade). */
typedef struct {
    float x;
    float y;
    float z;
} sensor_bno085_sample_t;

/**
 * Inicializa o barramento I2C, o driver do BNO085 (endereço 0x4A, confirmado
 * via i2cdetect) e habilita o report de Accelerometer calibrado a 100Hz (RNF-01).
 */
esp_err_t sensor_bno085_init(void);

/**
 * Bombeia o protocolo SH-2 (deve ser chamado periodicamente, ~10ms,
 * recomendação do driver) e atualiza a última amostra em cache.
 */
void sensor_bno085_poll(void);

/** Copia a última amostra recebida do sensor para 'out'. */
esp_err_t sensor_bno085_get_latest(sensor_bno085_sample_t *out);