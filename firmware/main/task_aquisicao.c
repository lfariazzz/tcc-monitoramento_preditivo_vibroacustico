#include "task_aquisicao.h"
#include "sensor_bno085.h"
#include "janela_dados.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

extern SemaphoreHandle_t g_sem_janela_pronta; // criado em main.c

// Buffer duplo (ping-pong): enquanto uma janela é preenchida, a outra
// pode estar sendo lida com segurança pelo consumidor (task_inferencia).
static janela_dados_t s_buffers[2];
static volatile uint8_t s_indice_escrita = 0;
static volatile uint8_t s_indice_pronto = 0;

const janela_dados_t* task_aquisicao_get_janela_pronta(void)
{
    return &s_buffers[s_indice_pronto];
}

void task_aquisicao(void *pvParameters)
{
    sensor_bno085_init();

    janela_dados_t *atual = &s_buffers[s_indice_escrita];
    atual->count = 0;

    const TickType_t periodo = pdMS_TO_TICKS(100);

    for (;;) {
        sensor_bno085_poll();

        sensor_bno085_sample_t amostra;
        if (sensor_bno085_get_latest(&amostra) == ESP_OK) {
            if (atual->count == 0) {
                atual->timestamp_inicio_us = esp_timer_get_time();
            }
            atual->amostras[atual->count++] = amostra;

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