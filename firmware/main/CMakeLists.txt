idf_component_register(
    SRCS "main.c" "task_aquisicao.c" "task_inferencia.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp-idf-lib__ads111x esp-idf-lib__i2cdev sensor_bno085
    PRIV_REQUIRES driver esp_timer actuator_buzzer actuator_led_rgb edge-impulse-sdk sensor_ky038
)