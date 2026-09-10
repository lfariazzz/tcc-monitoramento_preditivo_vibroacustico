#include "actuator_buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    actuator_buzzer_init();
    for (;;) {
        actuator_buzzer_on();
        vTaskDelay(pdMS_TO_TICKS(500));
        actuator_buzzer_off();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}