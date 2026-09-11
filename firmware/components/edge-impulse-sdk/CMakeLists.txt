set(EI_SDK_FOLDER ${CMAKE_CURRENT_LIST_DIR})
set(MODEL_FOLDER ${CMAKE_CURRENT_LIST_DIR}/..)

# Segue o padrão oficial do export Edge Impulse para ESP-IDF:
# github.com/edgeimpulse/example-standalone-inferencing-espressif-esp32/blob/main/main/CMakeLists.txt
if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    add_definitions(-DEI_CLASSIFIER_TFLITE_ENABLE_ESP_NN=1)
    add_definitions(-DEIDSP_USE_ESP_DSP=1)
    if(${IDF_TARGET} STREQUAL "esp32s3")
        add_definitions(-DEI_CLASSIFIER_TFLITE_ENABLE_ESP_NN_S3=1)
    endif()
endif()

include(${EI_SDK_FOLDER}/cmake/utils.cmake)

# Cada porting/*/ei_classifier_porting.cpp é guardado por #if EI_PORTING_<PLATAFORMA>,
# então compilar todas as plataformas é seguro: só a do Espressif gera código.
RECURSIVE_FIND_FILE_EXCLUDE_DIR(CPP_FILES "${EI_SDK_FOLDER}" "CMSIS" "*.cpp")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(CC_FILES  "${EI_SDK_FOLDER}" "CMSIS" "*.cc")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(C_FILES   "${EI_SDK_FOLDER}" "CMSIS" "*.c")
# Os fontes de assembly do ESP-NN/esp-dsp usam extensão maiúscula ".S"
# (precisam do preprocessador); GLOB é case-sensitive no Linux, então busca as duas.
RECURSIVE_FIND_FILE_EXCLUDE_DIR(S_FILES_ESP_NN     "${EI_SDK_FOLDER}/porting/espressif/ESP-NN" "CMSIS" "*.s")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(S_FILES_ESP_NN_CAP "${EI_SDK_FOLDER}/porting/espressif/ESP-NN" "CMSIS" "*.S")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(S_FILES_ESP_DSP     "${EI_SDK_FOLDER}/porting/espressif/esp-dsp/modules/fft" "CMSIS" "*.s")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(S_FILES_ESP_DSP_CAP "${EI_SDK_FOLDER}/porting/espressif/esp-dsp/modules/fft" "CMSIS" "*.S")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(MODEL_CPP_FILES "${MODEL_FOLDER}/tflite-model" "CMSIS" "*.cpp")

set(EI_SDK_SOURCES ${CPP_FILES} ${CC_FILES} ${C_FILES} ${S_FILES_ESP_NN} ${S_FILES_ESP_NN_CAP} ${S_FILES_ESP_DSP} ${S_FILES_ESP_DSP_CAP})
set(EI_MODEL_SOURCES ${MODEL_CPP_FILES})

idf_component_register(
    SRCS ${EI_SDK_SOURCES} ${EI_MODEL_SOURCES}
    INCLUDE_DIRS
        ".."
        "."
        "CMSIS/DSP/Include"
        "CMSIS/DSP/PrivateInclude"
        "CMSIS/Core/Include"
        "CMSIS/NN/Include"
        "third_party"
        "third_party/flatbuffers/include"
        "third_party/gemmlowp"
        "third_party/ruy"
        "tensorflow"
        "../tflite-model"
        "../model-parameters"
        "porting/espressif/esp-dsp/modules/common/include"
        "porting/espressif/esp-dsp/modules/fft/include"
    REQUIRES esp_timer
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++17)
