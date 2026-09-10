idf_component_register(
    SRCS "actuator_led_rgb.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio
)