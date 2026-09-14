#include "sensor_ky038.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "i2cdev.h"
#include "ads111x.h"

#define KY038_D0_GPIO      3

#define ADS1115_SDA_GPIO   6
#define ADS1115_SCL_GPIO   7
#define ADS1115_ADDR       ADS111X_ADDR_GND  // 0x48

static volatile uint32_t s_contador_picos = 0;
static i2c_dev_t s_ads1115_dev = { 0 };

static void IRAM_ATTR isr_ky038(void *arg)
{
    s_contador_picos++;
}

static esp_err_t init_d0(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << KY038_D0_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    esp_err_t err = gpio_config(&cfg);
    if (err != ESP_OK) return err;

    err = gpio_install_isr_service(0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;

    return gpio_isr_handler_add(KY038_D0_GPIO, isr_ky038, NULL);
}

static esp_err_t init_a0(void)
{
    esp_err_t err = i2cdev_init();
    if (err != ESP_OK) return err;

    err = ads111x_init_desc(&s_ads1115_dev, ADS1115_ADDR, I2C_NUM_0,
                             ADS1115_SDA_GPIO, ADS1115_SCL_GPIO);
    if (err != ESP_OK) return err;

    err = ads111x_set_mode(&s_ads1115_dev, ADS111X_MODE_CONTINUOUS);
    if (err != ESP_OK) return err;

    err = ads111x_set_input_mux(&s_ads1115_dev, ADS111X_MUX_0_GND);
    if (err != ESP_OK) return err;

    // Ganho padrão (±2.048V) — o único que não saturou nos testes de
    // bancada; sensibilidade fina compensada pelo ajuste do trimpot
    // físico do módulo (repouso calibrado em ~10000).
    return ads111x_set_gain(&s_ads1115_dev, ADS111X_GAIN_2V048);
}

esp_err_t sensor_ky038_init(void)
{
    esp_err_t err = init_d0();
    if (err != ESP_OK) return err;

    return init_a0();
}

uint32_t sensor_ky038_pico_count_reset(void)
{
    portDISABLE_INTERRUPTS();
    uint32_t contador = s_contador_picos;
    s_contador_picos = 0;
    portENABLE_INTERRUPTS();
    return contador;
}

esp_err_t sensor_ky038_ler_som(int16_t *valor_bruto)
{
    return ads111x_get_value(&s_ads1115_dev, valor_bruto);
}