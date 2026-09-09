#pragma once
#include "janela_dados.h"

void task_aquisicao(void *pvParameters);

/** Retorna a janela mais recente pronta para leitura. Chamar logo após
 *  receber o semáforo g_sem_janela_pronta. */
const janela_dados_t* task_aquisicao_get_janela_pronta(void);