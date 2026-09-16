idf_component_register(
    SRCS "connectivity_mqtt.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event nvs_flash mqtt esp_netif
)