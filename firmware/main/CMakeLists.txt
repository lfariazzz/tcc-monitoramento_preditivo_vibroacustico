idf_component_register(
    SRCS "main.c" "task_aquisicao.c" "task_inferencia.cpp"
    INCLUDE_DIRS "."
    REQUIRES sensor_bno085 sensor_ky038 actuator_buzzer actuator_led_rgb actuator_relay connectivity_mqtt edge-impulse-sdk
)

target_include_directories(${COMPONENT_LIB} PRIVATE
    "${CMAKE_SOURCE_DIR}/components"
)