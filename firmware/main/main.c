#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_bno085.h"
#include "sensor_ky038.h"
#include "actuator_led_rgb.h"
#include "actuator_buzzer.h"

void app_main(void)
{
    // KY-038 é inicializado primeiro: ele cria o barramento I2C
    // compartilhado (via i2cdev) que o BNO085 reaproveita depois,
    // já que os dois sensores estão no mesmo barramento (SDA=6, SCL=7).
    esp_err_t err_ky  = sensor_ky038_init();
    esp_err_t err_bno = sensor_bno085_init();
    esp_err_t err_led = actuator_led_rgb_init();
    esp_err_t err_buz = actuator_buzzer_init();

    printf("Init: bno=%s ky=%s led=%s buzzer=%s\n",
           esp_err_to_name(err_bno), esp_err_to_name(err_ky),
           esp_err_to_name(err_led), esp_err_to_name(err_buz));

    if (err_bno != ESP_OK || err_ky != ESP_OK) {
        printf("Falha critica na inicializacao dos sensores.\n");
        return;
    }

    // Ciclo de teste visual do LED, uma vez no início, pra confirmar
    // que ele responde mesmo com tudo mais inicializado junto
    actuator_led_rgb_set(LED_SEVERIDADE_NORMAL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    actuator_led_rgb_set(LED_SEVERIDADE_LEVE);
    vTaskDelay(pdMS_TO_TICKS(1000));
    actuator_led_rgb_set(LED_SEVERIDADE_SEVERA);
    vTaskDelay(pdMS_TO_TICKS(1000));
    actuator_led_rgb_set(LED_PARADO);

    printf("Lendo sensores continuamente (BNO085 + KY-038)...\n");

    for (;;) {
        sensor_bno085_poll();

        sensor_bno085_sample_t bno_amostra;
        sensor_bno085_get_latest(&bno_amostra);

        int16_t som_bruto = 0;
        sensor_ky038_ler_som(&som_bruto);

        uint32_t picos = sensor_ky038_pico_count_reset();

        printf("x=%.3f y=%.3f z=%.3f | som=%d | picos=%lu\n",
               bno_amostra.x, bno_amostra.y, bno_amostra.z,
               som_bruto, picos);

        // Buzzer soa brevemente se detectar qualquer pico de som —
        // só pra confirmar reação em tempo real, sem lógica de severidade
        if (picos > 0) {
            actuator_buzzer_on();
        } else {
            actuator_buzzer_off();
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}