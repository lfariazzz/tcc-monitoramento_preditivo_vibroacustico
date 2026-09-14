idf_component_register(
    SRCS "sensor_ky038.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio esp_timer esp-idf-lib__ads111x esp-idf-lib__i2cdev
)