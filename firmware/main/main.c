#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "connectivity_mqtt.h"

void app_main(void)
{
    esp_err_t err = connectivity_mqtt_init();
    printf("Init connectivity_mqtt: %s\n", esp_err_to_name(err));

    for (;;) {
        connectivity_mqtt_publicar_severidade("Teste");
        printf("Tentativa de publicacao enviada\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}