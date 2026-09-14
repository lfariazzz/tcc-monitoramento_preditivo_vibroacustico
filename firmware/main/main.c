#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_ky038.h"

void app_main(void)
{
    esp_err_t err = sensor_ky038_init();
    if (err != ESP_OK) {
        printf("Falha ao inicializar KY-038: %s\n", esp_err_to_name(err));
        return;
    }

    printf("KY-038 inicializado. Lendo D0 (picos) e A0 (som fino)...\n");

    for (;;) {
        uint32_t picos = sensor_ky038_pico_count_reset();

        int16_t som;
        esp_err_t err_leitura = sensor_ky038_ler_som(&som);

        if (err_leitura == ESP_OK) {
            printf("Picos: %lu | Som (bruto): %d\n", picos, som);
        } else {
            printf("Picos: %lu | Erro na leitura de som: %s\n",
                   picos, esp_err_to_name(err_leitura));
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}