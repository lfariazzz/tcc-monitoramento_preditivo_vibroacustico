idf_component_register(
    SRCS "main.c" "task_aquisicao.c"
    INCLUDE_DIRS "."
    REQUIRES sensor_bno085
    PRIV_REQUIRES esp_timer actuator_buzzer actuator_led_rgb
)