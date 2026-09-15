#pragma once
#include <stdint.h>
#include <stddef.h>

// Ajustado para bater com EI_CLASSIFIER_RAW_SAMPLE_COUNT do modelo v2
// (5 canais: x, y, z, som, picos — 100 amostras, 100Hz, 1s de janela)
#define JANELA_N_AMOSTRAS 100

typedef struct {
    float x;
    float y;
    float z;
    float som;      // valor bruto do ADS1115 (A0)
    float picos;    // nº de picos do D0 detectados desde o tick anterior
} amostra_combinada_t;

typedef struct {
    amostra_combinada_t amostras[JANELA_N_AMOSTRAS];
    size_t count;
    uint64_t timestamp_inicio_us;
} janela_dados_t;