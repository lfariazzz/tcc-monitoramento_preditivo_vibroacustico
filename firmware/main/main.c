#include "actuator_led_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    actuator_led_rgb_init();
    for (;;) {
        actuator_led_rgb_set(LED_SEVERIDADE_NORMAL);
        vTaskDelay(pdMS_TO_TICKS(1000));
        actuator_led_rgb_set(LED_SEVERIDADE_LEVE);
        vTaskDelay(pdMS_TO_TICKS(1000));
        actuator_led_rgb_set(LED_SEVERIDADE_SEVERA);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}