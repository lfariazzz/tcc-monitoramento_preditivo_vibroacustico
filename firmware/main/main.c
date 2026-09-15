#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_bno085.h"
#include "sensor_ky038.h"

void app_main(void)
{
    esp_err_t err_ky  = sensor_ky038_init();   // inicializa o i2cdev primeiro
    esp_err_t err_bno = sensor_bno085_init();  // agora o barramento já existe

    if (err_bno != ESP_OK || err_ky != ESP_OK) {
        printf("Falha na inicializacao: bno=%s ky=%s\n",
               esp_err_to_name(err_bno), esp_err_to_name(err_ky));
        return;
    }

    printf("x,y,z,som,picos\n");  // cabecalho CSV

    for (;;) {
        sensor_bno085_poll();

        sensor_bno085_sample_t bno_amostra;
        sensor_bno085_get_latest(&bno_amostra);

        int16_t som_bruto = 0;
        sensor_ky038_ler_som(&som_bruto);

        uint32_t picos = sensor_ky038_pico_count_reset();

        printf("%.4f,%.4f,%.4f,%d,%lu\n",
               bno_amostra.x, bno_amostra.y, bno_amostra.z,
               som_bruto, picos);

        vTaskDelay(pdMS_TO_TICKS(10));  // 100Hz — RNF-01/02
    }
}