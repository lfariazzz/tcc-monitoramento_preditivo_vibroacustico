#pragma once
#include "esp_err.h"

/**
 * Inicializa NVS, Wi-Fi (modo station, reconexão automática) e o cliente
 * MQTT, conectando ao broker configurado via Kconfig (RF-09).
 * Não bloqueia indefinidamente em caso de falha de rede — segue tentando
 * reconectar em segundo plano, sem travar o restante do sistema
 * (RNF-10 / "Estabilidade e Operacionalidade Mínima").
 */
esp_err_t connectivity_mqtt_init(void);

/**
 * Publica o estado de severidade atual no tópico MQTT.
 * Não-bloqueante; se o MQTT não estiver conectado no momento, a chamada
 * apenas loga um aviso e retorna, sem afetar o restante do pipeline.
 */
void connectivity_mqtt_publicar_severidade(const char *severidade);