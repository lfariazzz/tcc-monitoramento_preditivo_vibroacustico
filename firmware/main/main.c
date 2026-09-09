#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_bno085.h"

void app_main(void)
{
    esp_err_t err = sensor_bno085_init();
    if (err != ESP_OK) {
        printf("Falha ao inicializar BNO085: %s\n", esp_err_to_name(err));
        return;
    }

    printf("BNO085 inicializado. Lendo...\n");

    for (;;) {
        sensor_bno085_poll();

        sensor_bno085_sample_t amostra;
        if (sensor_bno085_get_latest(&amostra) == ESP_OK) {
            printf("%.4f, %.4f, %.4f\n", amostra.x, amostra.y, amostra.z);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}