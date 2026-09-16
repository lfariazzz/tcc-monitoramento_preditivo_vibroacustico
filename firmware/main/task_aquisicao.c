#include "task_aquisicao.h"
#include "sensor_bno085.h"
#include "sensor_ky038.h"
#include "janela_dados.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

extern SemaphoreHandle_t g_sem_janela_pronta;

static janela_dados_t s_buffers[2];
static volatile uint8_t s_indice_escrita = 0;
static volatile uint8_t s_indice_pronto = 0;

const janela_dados_t* task_aquisicao_get_janela_pronta(void)
{
    return &s_buffers[s_indice_pronto];
}

void task_aquisicao(void *pvParameters)
{
    sensor_ky038_init();
    sensor_bno085_init();

    janela_dados_t *atual = &s_buffers[s_indice_escrita];
    atual->count = 0;

    const TickType_t periodo = pdMS_TO_TICKS(10); // 100Hz — bate com o modelo v2

    for (;;) {
        sensor_bno085_poll();

        sensor_bno085_sample_t bno_amostra;
        esp_err_t err_bno = sensor_bno085_get_latest(&bno_amostra);

        int16_t som_bruto = 0;
        esp_err_t err_som = sensor_ky038_ler_som(&som_bruto);

        uint32_t picos = sensor_ky038_pico_count_reset();

        if (err_bno == ESP_OK && err_som == ESP_OK) {
            if (atual->count == 0) {
                atual->timestamp_inicio_us = esp_timer_get_time();
            }

            atual->amostras[atual->count].x = bno_amostra.x;
            atual->amostras[atual->count].y = bno_amostra.y;
            atual->amostras[atual->count].z = bno_amostra.z;
            atual->amostras[atual->count].som = (float)som_bruto;
            atual->amostras[atual->count].picos = (float)picos;
            atual->count++;

            if (atual->count >= JANELA_N_AMOSTRAS) {
                s_indice_pronto = s_indice_escrita;
                s_indice_escrita = 1 - s_indice_escrita;
                atual = &s_buffers[s_indice_escrita];
                atual->count = 0;

                xSemaphoreGive(g_sem_janela_pronta);
            }
        }

        vTaskDelay(periodo);
    }
}