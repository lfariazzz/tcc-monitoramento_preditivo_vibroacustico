// Ponto de entrada do firmware: cria o semáforo de sincronização e as duas
// tasks do pipeline (task_aquisicao produz janelas de amostras; task_inferencia
// consome cada janela pronta, roda a inferência e decide severidade/corte do
// relé).
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "task_aquisicao.h"
#include "task_inferencia.h"

SemaphoreHandle_t g_sem_janela_pronta;

void app_main(void)
{
    g_sem_janela_pronta = xSemaphoreCreateBinary();

    xTaskCreate(task_aquisicao, "task_aquisicao", 8192, NULL, 5, NULL);
    xTaskCreate(task_inferencia, "task_inferencia", 16384, NULL, 5, NULL);
}