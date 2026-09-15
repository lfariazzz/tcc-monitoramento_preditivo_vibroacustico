#include "sensor_bno085.h"
#include "bno085.h"
#include "driver/i2c_master.h"
#include "i2cdev.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "sensor_bno085";

// TODO(hardware): RST e INT ainda precisam ser movidos de fiação fixa
// para GPIOs reais na bancada — ver alerta na issue #15.
#define BNO085_I2C_SDA_GPIO   6
#define BNO085_I2C_SCL_GPIO   7
#define BNO085_I2C_ADDR       0x4A
#define BNO085_INT_GPIO       5
#define BNO085_RESET_GPIO     4
#define BNO085_REPORT_HZ      100 // RNF-01: mínimo 100Hz

static bno085_handle_t s_handle = NULL;
static i2c_master_bus_handle_t s_bus = NULL;
static sensor_bno085_sample_t s_latest = {0};

static void accel_callback(bno085_handle_t handle,
                            const bno085_sensor_value_t *value,
                            void *ctx)
{
    if (value->sensor_id == BNO085_SENSOR_ACCELEROMETER) {
        s_latest.x = value->data.accelerometer.x;
        s_latest.y = value->data.accelerometer.y;
        s_latest.z = value->data.accelerometer.z;
    }
}

esp_err_t sensor_bno085_init(void)
{
    // BNO085 e KY-038/ADS1115 compartilham o mesmo barramento físico
    // (SDA=6, SCL=7, I2C_NUM_0). O sensor_ky038 é inicializado primeiro
    // e cria o barramento via i2cdev; aqui só reaproveitamos o handle
    // já instalado em vez de chamar i2c_new_master_bus de novo, o que
    // falharia com ESP_ERR_INVALID_STATE (porta já adquirida).
    esp_err_t err = i2cdev_get_shared_handle(I2C_NUM_0, (void **)&s_bus);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao obter barramento I2C compartilhado: %s", esp_err_to_name(err));
        return err;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BNO085_I2C_ADDR,
        .scl_speed_hz = 400000,
    };
    i2c_master_dev_handle_t dev;
    err = i2c_master_bus_add_device(s_bus, &dev_cfg, &dev);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao adicionar dispositivo I2C: %s", esp_err_to_name(err));
        return err;
    }

    err = bno085_init(NULL, dev, BNO085_INT_GPIO, BNO085_RESET_GPIO, &s_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar BNO085: %s", esp_err_to_name(err));
        return err;
    }

    bno085_register_sensor_callback(s_handle, accel_callback, NULL);

    uint32_t interval_us = 1000000 / BNO085_REPORT_HZ;
    return bno085_enable_sensor(s_handle, BNO085_SENSOR_ACCELEROMETER, interval_us);
}

void sensor_bno085_poll(void)
{
    if (s_handle) {
        bno085_service(s_handle);
    }
}

esp_err_t sensor_bno085_get_latest(sensor_bno085_sample_t *out)
{
    if (!out) return ESP_ERR_INVALID_ARG;
    *out = s_latest;
    return ESP_OK;
}