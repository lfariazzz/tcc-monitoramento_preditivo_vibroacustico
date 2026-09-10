idf_component_register(
    SRCS "actuator_buzzer.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio
)