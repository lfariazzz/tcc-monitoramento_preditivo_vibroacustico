extern "C" {
#include "task_inferencia.h"
#include "janela_dados.h"
#include "actuator_led_rgb.h"
#include "actuator_buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_log.h"
}
#include <string.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

static const char *TAG = "task_inferencia";

extern SemaphoreHandle_t g_sem_janela_pronta;
extern "C" const janela_dados_t* task_aquisicao_get_janela_pronta(void);

// RNF-05: tempo de persistência de anomalia severa antes de acionar o relé
#define TEMPO_PERSISTENCIA_SEVERA_MS   5000

static int64_t s_inicio_severa_us = 0;
static bool s_em_severa = false;
static bool s_rele_acionado = false;

// TODO: substituir por actuator_relay real quando esse componente existir (RF-08)
static void actuator_relay_on(void)
{
    ESP_LOGW(TAG, "RELÉ ACIONADO (corte de energia) — RF-08");
}

static void avaliar_persistencia_severa(bool esta_severa)
{
    int64_t agora_us = esp_timer_get_time();

    if (esta_severa) {
        if (!s_em_severa) {
            s_em_severa = true;
            s_inicio_severa_us = agora_us;
            s_rele_acionado = false;
        } else if (!s_rele_acionado) {
            int64_t duracao_ms = (agora_us - s_inicio_severa_us) / 1000;
            if (duracao_ms >= TEMPO_PERSISTENCIA_SEVERA_MS) {
                actuator_relay_on();
                s_rele_acionado = true;
            }
        }
    } else {
        // Saiu de severa antes do limiar: zera a contagem (RNF-05)
        s_em_severa = false;
        s_rele_acionado = false;
    }
}

void task_inferencia(void *pvParameters)
{
    actuator_buzzer_init();
    actuator_led_rgb_init();

    for (;;) {
        if (xSemaphoreTake(g_sem_janela_pronta, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        const janela_dados_t *janela = task_aquisicao_get_janela_pronta();

        // sensor_bno085_sample_t {x,y,z} é contíguo em memória — já é o
        // formato plano [x0,y0,z0,x1,y1,z1,...] que o Edge Impulse espera.
        signal_t signal;
        numpy::signal_from_buffer(
            (float *)janela->amostras,
            EI_CLASSIFIER_RAW_SAMPLE_COUNT * EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME,
            &signal);

        ei_impulse_result_t result = { 0 };
        EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
        if (err != EI_IMPULSE_OK) {
            ESP_LOGE(TAG, "Falha na inferência: %d", err);
            continue;
        }

        float melhor_valor = 0;
        const char *melhor_label = "";
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            if (result.classification[ix].value > melhor_valor) {
                melhor_valor = result.classification[ix].value;
                melhor_label = result.classification[ix].label;
            }
        }

        ESP_LOGI(TAG, "Classe: %s (%.2f)", melhor_label, melhor_valor);

        // ⚠️ Nomes de label ainda não confirmados byte a byte — ver nota abaixo
        led_severidade_t severidade;
        bool anomalia = false;

        if (strcmp(melhor_label, "Normal") == 0) {
            severidade = LED_SEVERIDADE_NORMAL;
        } else if (strcmp(melhor_label, "Anomalia Leve") == 0) {
            severidade = LED_SEVERIDADE_LEVE;
            anomalia = true;
        } else {
            severidade = LED_SEVERIDADE_SEVERA;
            anomalia = true;
        }

        actuator_led_rgb_set(severidade);
        anomalia ? actuator_buzzer_on() : actuator_buzzer_off();

        avaliar_persistencia_severa(severidade == LED_SEVERIDADE_SEVERA);
    }
}