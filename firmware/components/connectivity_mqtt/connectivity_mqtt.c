#include "connectivity_mqtt.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include <string.h>

static const char *TAG = "connectivity_mqtt";

static esp_mqtt_client_handle_t s_mqtt_client = NULL;
static bool s_mqtt_conectado = false;

#define MQTT_TOPIC  "pnaat/monitoramento/severidade"

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Wi-Fi desconectado — tentando reconectar...");
        esp_wifi_connect(); // reconexão automática, sem bloquear o sistema
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Wi-Fi conectado, IP obtido");
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                                int32_t event_id, void *event_data)
{
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT conectado ao broker");
            s_mqtt_conectado = true;
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT desconectado");
            s_mqtt_conectado = false;
            break;
        default:
            break;
    }
}

static esp_err_t iniciar_wifi(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) return err;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                 &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                 &wifi_event_handler, NULL));

    wifi_config_t wifi_config = { 0 };
    strncpy((char *)wifi_config.sta.ssid, CONFIG_WIFI_SSID, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, CONFIG_WIFI_PASSWORD, sizeof(wifi_config.sta.password) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

static esp_err_t iniciar_mqtt(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
    };

    s_mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (s_mqtt_client == NULL) {
        ESP_LOGE(TAG, "Falha ao inicializar cliente MQTT");
        return ESP_FAIL;
    }

    esp_mqtt_client_register_event(s_mqtt_client, MQTT_EVENT_ANY,
                                     mqtt_event_handler, NULL);

    return esp_mqtt_client_start(s_mqtt_client);
}

esp_err_t connectivity_mqtt_init(void)
{
    esp_err_t err = iniciar_wifi();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao iniciar Wi-Fi: %s", esp_err_to_name(err));
        return err;
    }

    return iniciar_mqtt();
}

void connectivity_mqtt_publicar_severidade(const char *severidade)
{
    if (!s_mqtt_conectado || s_mqtt_client == NULL) {
        ESP_LOGW(TAG, "MQTT não conectado — evento não publicado: %s", severidade);
        return;
    }

    char payload[64];
    snprintf(payload, sizeof(payload), "{\"severidade\":\"%s\"}", severidade);

    // qos=0, retain=0, não-bloqueante
    esp_mqtt_client_publish(s_mqtt_client, MQTT_TOPIC, payload, 0, 0, 0);
}