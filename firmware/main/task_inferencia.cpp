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
#include <math.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

static const char *TAG = "task_inferencia";

extern SemaphoreHandle_t g_sem_janela_pronta;
extern "C" const janela_dados_t* task_aquisicao_get_janela_pronta(void);

// RNF-05: tempo de persistência de anomalia severa antes de acionar o relé
#define TEMPO_PERSISTENCIA_SEVERA_MS   5000

// Duração do beep breve para anomalia LEVE — depois disso, desliga sozinho
// mesmo que a condição leve persista. SEVERA continua contínua (sem limite).
#define DURACAO_BEEP_LEVE_MS   500

// Detecção de "parado" via threshold simples — decisão de firmware,
// não do modelo de IA (motor ligado/desligado é regra determinística,
// não padrão de vibração a ser reconhecido por ML).
//
// TODO: calibrar este valor com dados reais de bancada, usando o
// monitor_vibracao_simples.py (motor desligado vs. ligado sem peso).
// Placeholder não validado.
#define LIMIAR_DESVIO_PARADO_MS2   0.5f

static int64_t s_inicio_severa_us = 0;
static bool s_em_severa = false;
static bool s_rele_acionado = false;

static int64_t s_inicio_beep_leve_us = 0;
static bool s_beep_leve_ativo = false;

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

static bool esta_parado(const janela_dados_t *janela)
{
    float soma = 0, soma_sq = 0;
    for (size_t i = 0; i < janela->count; i++) {
        float x = janela->amostras[i].x;
        float y = janela->amostras[i].y;
        float z = janela->amostras[i].z;
        float mag = sqrtf(x * x + y * y + z * z);
        soma += mag;
        soma_sq += mag * mag;
    }
    float media = soma / janela->count;
    float variancia = (soma_sq / janela->count) - (media * media);
    float desvio = sqrtf(variancia > 0 ? variancia : 0);

    return desvio < LIMIAR_DESVIO_PARADO_MS2;
}

// Decide o comportamento do buzzer conforme a severidade:
// - SEVERA: contínuo, sem desligar sozinho, enquanto a condição persistir.
// - LEVE: um beep breve (DURACAO_BEEP_LEVE_MS) e desliga sozinho, mesmo que
//         a condição leve continue nas próximas janelas.
// - NORMAL/outros: desligado, e reseta o estado do beep breve.
static void controlar_buzzer(led_severidade_t severidade)
{
    if (severidade == LED_SEVERIDADE_SEVERA) {
        actuator_buzzer_on();
        s_beep_leve_ativo = false;
    } else if (severidade == LED_SEVERIDADE_LEVE) {
        if (!s_beep_leve_ativo) {
            actuator_buzzer_on();
            s_inicio_beep_leve_us = esp_timer_get_time();
            s_beep_leve_ativo = true;
        } else {
            int64_t duracao_ms = (esp_timer_get_time() - s_inicio_beep_leve_us) / 1000;
            if (duracao_ms >= DURACAO_BEEP_LEVE_MS) {
                actuator_buzzer_off();
            }
        }
    } else {
        actuator_buzzer_off();
        s_beep_leve_ativo = false;
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

        // Checagem de "parado" — regra de firmware, roda ANTES da inferência.
        // Se o motor não está em movimento, não há motivo para gastar ciclos
        // rodando o modelo de IA.
        if (esta_parado(janela)) {
            ESP_LOGI(TAG, "Motor parado (desvio abaixo do limiar)");
            actuator_led_rgb_set(LED_PARADO);
            actuator_buzzer_off();
            s_beep_leve_ativo = false;
            s_em_severa = false;
            s_rele_acionado = false;
            continue;
        }

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

        if (strcmp(melhor_label, "Normal") == 0) {
            severidade = LED_SEVERIDADE_NORMAL;
        } else if (strcmp(melhor_label, "Anomalia Leve") == 0) {
            severidade = LED_SEVERIDADE_LEVE;
        } else {
            severidade = LED_SEVERIDADE_SEVERA;
        }

        actuator_led_rgb_set(severidade);
        controlar_buzzer(severidade);

        avaliar_persistencia_severa(severidade == LED_SEVERIDADE_SEVERA);
    }
}