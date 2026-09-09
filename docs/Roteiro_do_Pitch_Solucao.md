# Roteiro do pitch — Solução tecnológica

**Projeto:** Monitoramento Preditivo Vibroacústico com Edge AI  
**Equipe:** Wayne Enterprises — Intensivo Maker (PNAAT)  
**Duração da solução:** 3min15s, com faixa de execução entre 3min e 3min30s.

## 1. Objetivo e referências

Apresentar a tecnologia escolhida e explicar como ela atua sobre o problema: transformar alterações de vibração e ruído em informação sobre a condição do equipamento, com decisão e resposta locais e acompanhamento remoto. A sequência deve aproveitar o encerramento da introdução e preparar o público para observar esse funcionamento na bancada.

Este documento detalha a **parte de solução**, conforme a descrição fornecida da issue, embora seu título mencione introdução. Inclui também uma proposta de integração das quatro partes para conferir os critérios de avaliação.

Bases utilizadas:

- [Roteiro da introdução](Roteiro_do_Pitch_Introdução.pdf): contexto, problema e passagem para a solução.
- [README do projeto](../README.md): arquitetura, componentes e escopo da prova de conceito.
- [Levantamento de requisitos](Entrega_1-Levantamento_de_Requisitos.pdf): RF-01 a RF-10, condição de atuação do relé em RNF-05 e limitações da solução.

**Status:** roteiro de planejamento. O firmware disponível contém a inicialização e tarefas ainda a implementar; a estrutura do dashboard ainda não contém o fluxo. As falas descrevem a proposta e os comportamentos esperados. Antes da apresentação final, conferir a implementação e as evidências de bancada para atualizar o que poderá ser demonstrado.

## 2. Encaixe na apresentação completa

Distribuição proposta para integrar o roteiro existente de introdução às demais partes. Os tempos da introdução, demonstração e conclusão são reservas de planejamento, a confirmar no ensaio.

| Parte | Tempo no pitch | Duração | Conteúdo e conexão narrativa |
|---|---|---|---|
| Introdução | 0:00–4:00 | 4min | Seguir o PDF: contexto industrial, degradação pouco perceptível, manutenção reativa ou por calendário e necessidade de acompanhamento por condição. Encerrar anunciando a integração dos componentes. |
| Solução | 4:00–7:15 | 3min15s | Explicar captura, classificação embarcada, resposta e registro. Encerrar anunciando os estados normal e anômalo que serão mostrados. |
| Demonstração | 7:15–12:15 | 5min | Mostrar o cooler, a leitura e classificação, a anomalia controlada, o alerta, a atuação do relé e os registros. Reservar 4min para a atividade prática, conforme a seção 5. |
| Conclusão | 12:15–14:00 | 1min45s | Retomar problema, solução, evidências observadas e resultado esperado, conforme a seção 6. |
| Margem | 14:00–15:00 | 1min | Absorver transições e pequenos atrasos, sem acrescentar conteúdo obrigatório. |

**Total de conteúdo: 14min. Limite com margem: 15min.** Se a solução ocupar 3min30s, os blocos seguintes se deslocam 15s e restam 45s de margem.

## 3. Estrutura da parte de solução

Os intervalos abaixo são relativos ao início desta parte. As trocas de slide e de apresentador estão incluídas nos tempos.

| Bloco | Intervalo | Duração | Mensagem central | Apoio visual |
|---|---|---|---|---|
| Gancho da introdução | 0:00–0:20 | 20s | O acompanhamento por condição exige converter os sinais em decisão. | Título e fluxo geral. |
| Captura dos sinais | 0:20–1:00 | 40s | Vibração e som fornecem dados sobre o funcionamento. | Destacar cooler, BNO085 e KY-038 com ADS1115. |
| Decisão na borda | 1:00–1:50 | 50s | O ESP32-S3 interpreta os dados com IA embarcada. | Destacar extração de características e classificação normal/anômalo. |
| Resposta e registro | 1:50–2:40 | 50s | A classificação orienta alerta, atuação e acompanhamento. | Destacar buzzer, relé, RTC/MicroSD e MQTT/painel. |
| Resultado e ponte | 2:40–3:15 | 35s | Delimitar a PoC e anunciar o que será observado. | Exibir “normal → anomalia → resposta → registro” e passar à bancada. |

Usar dois slides: o primeiro acompanha os três primeiros blocos; o segundo acompanha resposta, registro e passagem para a demonstração. No primeiro, o fluxo pode ser apresentado como:

**Equipamento → sensores → ESP32-S3 / IA → classificação normal ou anômala.**

No segundo, abrir as saídas da decisão em **alerta/relé**, **registro local** e **comunicação/painel**. Evidenciar que a rede não é uma etapa necessária para a decisão local.

## 4. Falas sugeridas

### 4.1. Gancho da introdução — 0:00 a 0:20

> Para realizar esse monitoramento de forma autônoma, organizamos a solução em três movimentos: captar os sinais da máquina, interpretar seu padrão e responder quando houver uma anomalia. Assim, aquela necessidade de acompanhar a condição real do equipamento se transforma em um fluxo de decisão baseado em dados.

### 4.2. Captura dos sinais — 0:20 a 1:00

> Na nossa bancada, um cooler representa o equipamento rotativo. O sensor BNO085 acompanha sua vibração, enquanto o KY-038 capta o som do funcionamento. A leitura analógica desse som passa pelo conversor ADS1115; a saída digital identifica picos sonoros que também entram na análise.
>
> Esses sinais chegam ao ESP32-S3, o microcontrolador que concentra o processamento. A proposta é transformar aquilo que pode passar despercebido em uma inspeção em medições contínuas. Vibração e som passam a fornecer dados para reconhecer mudanças no padrão de funcionamento.

### 4.3. Decisão na borda — 1:00 a 1:50

> Mas coletar dados, por si só, não informa se a máquina precisa de atenção. Por isso, o sistema organiza as leituras em janelas e extrai características que representam o comportamento do equipamento. Um modelo treinado previamente com padrões normais e anômalos, usando o Edge Impulse, será embarcado para classificar essas janelas.
>
> É aí que entra a Edge AI: a análise acontece no próprio ESP32-S3, ao lado da máquina. A decisão local não precisa esperar uma resposta da internet. O projeto também prevê separar as tarefas de aquisição, inferência e comunicação com FreeRTOS, para manter o monitoramento funcionando mesmo quando a rede estiver indisponível.

### 4.4. Resposta e registro — 1:50 a 2:40

> A classificação orienta a resposta. Quando uma anomalia é identificada, o sistema deve emitir um alerta pelo buzzer. Se ela aparecer em três classificações consecutivas, está previsto o corte da alimentação do equipamento de teste por um relé. Essa condição de persistência diferencia o alerta inicial da atuação sobre o motor.
>
> Para acompanhamento, o projeto prevê enviar estados e eventos por MQTT a um painel Node-RED. O RTC fornece a referência de data e hora, e o MicroSD armazena os registros localmente. Sem rede, a aquisição, a análise e a resposta local devem continuar; quando a conexão voltar, os eventos pendentes devem ser reenviados em ordem cronológica.

### 4.5. Resultado esperado e ponte para a demonstração — 2:40 a 3:15

> Com esse fluxo, buscamos oferecer evidências para que a manutenção possa agir diante de uma mudança de condição, reduzindo o risco de intervenções tardias. Nesta prova de conceito, o foco é reconhecer padrões conhecidos em um equipamento de bancada, dentro das condições de teste; o sistema não calcula o tempo exato até uma falha.
>
> Agora vamos acompanhar esse caminho na prática: observar o funcionamento normal, apresentar uma condição anômala controlada e verificar a classificação, o alerta, a atuação do relé e o registro do evento.

## 5. Ponte operacional para a demonstração

**Abertura sugerida do próximo apresentador:** “Vamos começar pelo estado normal do cooler e acompanhar o mesmo fluxo que acabamos de apresentar: captura, classificação, resposta e registro.”

Planejamento dos 5min reservados, com bancada montada e painel aberto antes do início do pitch:

| Tempo relativo | Atividade | Evidência a mostrar |
|---|---|---|
| 0:00–0:30 | Identificar cooler, sensores, controlador e saídas. | Correspondência entre diagrama e montagem. |
| 0:30–1:15 | Operar o cooler na condição normal definida no ensaio. | Leituras e classificação normal no monitor/painel. |
| 1:15–2:45 | Executar o cenário de anomalia controlada previamente validado. | Mudança de classificação, alerta sonoro e corte após três classificações anômalas consecutivas. |
| 2:45–3:30 | Mostrar a publicação e o registro do evento. | Estado/evento recebido no painel e registro local com data e hora. |
| 3:30–4:30 | Simular indisponibilidade de rede e reconexão, retomando a bancada conforme o procedimento ensaiado. | Continuidade local, registro offline e reenvio dos eventos pendentes. |
| 4:30–5:00 | Relacionar o observado ao objetivo e passar à conclusão. | Síntese das evidências efetivamente obtidas. |

**Atividade prática: 0:30–4:30, total de 4min.** Manter bancada e painel simultaneamente visíveis. Definir no ensaio o cenário de anomalia, a sequência de retomada após o corte e os responsáveis por fala e operação.

O teste breve de desconexão ilustra o funcionamento previsto; a validação de 30min offline do RNF-11 deve ocorrer separadamente. Se houver falha ao vivo, usar uma gravação real previamente produzida e identificada como tal, dentro do mesmo intervalo. Funcionalidades ainda não implementadas devem ser identificadas como previstas; animações ou simulações não devem ser apresentadas como evidência de funcionamento.

## 6. Estrutura de conclusão para manter a continuidade

| Tempo relativo | Conteúdo | Orientação de fala |
|---|---|---|
| 0:00–0:25 | Retomar o problema. | “Começamos com o desafio de perceber mudanças de condição antes que a manutenção seja surpreendida por uma falha.” |
| 0:25–1:00 | Retomar a solução e o observado. | Relacionar sensores, classificação local e resposta às evidências que a demonstração efetivamente apresentou. |
| 1:00–1:30 | Explicitar o resultado esperado. | Apoiar decisões de manutenção por condição e contribuir para reduzir paradas inesperadas e intervenções desnecessárias. Apresentar esses benefícios como objetivos, sem atribuir percentuais de redução ao protótipo. |
| 1:30–1:45 | Encerrar com o alcance da entrega. | “A proposta transforma vibração e som em informação para agir. A bancada delimita esta prova de conceito e fornece a base para avaliar sua evolução.” |

## 7. Conferência dos critérios de pontuação

| Nível | Critério fornecido | Evidência no planejamento |
|---|---|---|
| Adequado | Permitir verificar o planejamento integral, com introdução, solução, demonstração e conclusão. | Seção 2 integra as quatro partes; o PDF detalha a introdução e as seções 3 a 6 detalham as demais. |
| Adequado | Introdução apresenta o problema. | PDF e seção 2: degradação pouco perceptível, paradas inesperadas e limitações da manutenção reativa ou por calendário. |
| Adequado | Solução indica como a tecnologia atua sobre o problema. | Seção 4 conecta medições, interpretação, decisão, resposta e acompanhamento à necessidade apresentada. |
| Adequado | Demonstração descreve o funcionamento e reserva espaço e tempo para a prática. | Seção 5 prevê bancada e painel visíveis, sequência observável e 4min de operação prática. |
| Adequado | Conclusão retoma problema, solução e resultado esperado. | Seção 6 reserva conteúdo e duração para cada retomada. |
| Adequado | Duração total de até 15min. | Seção 2: 14min de conteúdo e 1min de margem. |
| Avançado | Informar conteúdo e tempo de cada parte. | Seção 2 e detalhamentos temporais nas seções 3, 5 e 6. |
| Avançado | Problema conduz à solução, solução à demonstração e conclusão retoma a narrativa. | Gancho em 4.1, passagem em 4.5, abertura em 5 e retomadas em 6. |
| Avançado | Distribuição permite apresentar as quatro partes dentro do limite. | Reservas por etapa e margem explícita; solução limitada a 3min–3min30s. |

**Conferência documental:** a estrutura contempla os itens dos níveis Adequado e Avançado. A nota depende da avaliação da entrega; o cumprimento do tempo e a execução prática precisam ser confirmados no ensaio.

### Checklist para o ensaio final

- [ ] Cronometrar a solução com pausas e slides: entre 3min e 3min30s, alvo de 3min15s.
- [ ] Cronometrar as quatro partes juntas: conteúdo em até 14min e apresentação encerrada em até 15min.
- [ ] Preservar o gancho da introdução e a passagem direta para os estados que serão demonstrados.
- [ ] Conferir a regra de três classificações anômalas consecutivas antes da atuação do relé.
- [ ] Ensaiar a atividade prática e a retomada da bancada dentro dos 4min reservados.
- [ ] Atualizar as falas conforme funcionalidades implementadas e resultados documentados; acurácia e latências previstas nos requisitos só podem ser anunciadas como medidas se houver evidências.
- [ ] Conferir novamente cada linha da tabela de pontuação após ajustes no roteiro.
