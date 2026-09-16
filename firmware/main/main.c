// main TEMPORÁRIA para validar o sistema completo (aquisição + inferência +
// relé) rodando junto na placa, no arranjo de tasks em que ele deve operar
// de fato (task_aquisicao produz janelas, task_inferencia consome e decide
// severidade/corte do relé). Antes disso o app_main só fazia log CSV bruto
// e nunca chegava a criar essas tasks nem o semáforo que as liga.
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