#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "task_aquisicao.h"
#include "janela_dados.h"

SemaphoreHandle_t g_sem_janela_pronta;

// TEMPORÁRIO: simula o que a task_inferencia (Task #16) fará de verdade.
// Serve só pra validar aquisição -> janela -> semáforo nesta issue (#15).
// Remover quando a Task #16 existir de fato.
static void task_consumidor_teste(void *pv)
{
    for (;;) {
        if (xSemaphoreTake(g_sem_janela_pronta, portMAX_DELAY) == pdTRUE) {
            const janela_dados_t *janela = task_aquisicao_get_janela_pronta();
            printf("Janela pronta: %d amostras, inicio=%llu us\n",
                   (int)janela->count, janela->timestamp_inicio_us);
            printf("  primeira amostra: %.4f, %.4f, %.4f\n",
                   janela->amostras[0].x, janela->amostras[0].y, janela->amostras[0].z);
            printf("  ultima amostra:   %.4f, %.4f, %.4f\n",
                   janela->amostras[janela->count - 1].x,
                   janela->amostras[janela->count - 1].y,
                   janela->amostras[janela->count - 1].z);
        }
    }
}

void app_main(void)
{
    g_sem_janela_pronta = xSemaphoreCreateBinary();

    xTaskCreate(task_aquisicao, "task_aquisicao", 4096, NULL, 5, NULL);
    xTaskCreate(task_consumidor_teste, "task_consumidor_teste", 4096, NULL, 5, NULL);
}