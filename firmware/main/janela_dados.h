#pragma once
#include "sensor_bno085.h"
#include <stdint.h>
#include <stddef.h>

// PROVISÓRIO: 1s de janela a 100Hz. Ajustar quando a Task #12 (Edge Impulse)
// definir o tamanho de janela real do modelo treinado.
#define JANELA_N_AMOSTRAS 100

typedef struct {
    sensor_bno085_sample_t amostras[JANELA_N_AMOSTRAS];
    size_t count;
    uint64_t timestamp_inicio_us;
} janela_dados_t;