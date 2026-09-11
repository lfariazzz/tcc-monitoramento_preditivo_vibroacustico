#pragma once
#include "sensor_bno085.h"
#include <stdint.h>
#include <stddef.h>

#define JANELA_N_AMOSTRAS 30

typedef struct {
    sensor_bno085_sample_t amostras[JANELA_N_AMOSTRAS];
    size_t count;
    uint64_t timestamp_inicio_us;
} janela_dados_t;