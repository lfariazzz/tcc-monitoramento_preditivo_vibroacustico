idf_component_register(
    SRCS "sensor_bno085.c"
    INCLUDE_DIRS "."
    REQUIRES bno085
    PRIV_REQUIRES driver esp_driver_i2c
)