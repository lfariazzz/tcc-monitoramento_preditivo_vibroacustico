# Roteiro do Pitch - Conclusão

## Monitoramento Preditivo Vibroacústico com Edge AI

**Duração prevista:** 2min30s

**Formato:** encerramento narrado, retomando problema, solução, demonstração e alcance da prova de conceito

**Objetivo:** conectar a necessidade apresentada na introdução, a solução tecnológica explicada em seguida e os comportamentos observados na demonstração. O encerramento deve reforçar o valor da manutenção orientada por condição, delimitar o que a PoC comprova e indicar possibilidades de evolução.

## Organização do espaço

A conclusão deve ocorrer com a bancada e o painel ainda visíveis, preferencialmente no último estado válido da demonstração.

1. **Bancada do protótipo:** manter o cooler, sensores, ESP32-S3 e atuadores visíveis.
2. **Tela de monitoramento:** manter o último estado ou evento demonstrado no painel.
3. **Slide de encerramento:** organizar quatro mensagens curtas: problema, solução, resultado observado e próximos passos.

O apresentador deve ficar em posição que permita apontar a bancada e o painel sem bloquear a visualização. Não é necessário repetir a demonstração; a conclusão interpreta o que foi mostrado.

## Roteiro detalhado

### 0:00-0:30 - Retomada do problema

**Ação:** exibir o slide final com o problema inicial e indicar brevemente a bancada.

**Fala sugerida:**

> Começamos com um desafio comum em equipamentos rotativos: os sinais de degradação podem surgir de forma gradual na vibração e no ruído, antes de uma falha mais grave, mas nem sempre são percebidos a tempo pela inspeção humana.
>
> Isso deixa a manutenção entre dois extremos: reagir somente depois da falha ou intervir por calendário, mesmo sem evidência de necessidade.

**Resultado visível esperado:** a banca reconhece a conexão entre o problema inicial e a necessidade de acompanhar a condição real do equipamento.

### 0:30-1:00 - Retomada da solução

**Ação:** apontar o caminho entre equipamento, sensores, processamento local e respostas do sistema.

**Fala sugerida:**

> Para atuar sobre esse problema, propusemos um sistema de monitoramento vibroacústico com Edge AI. A ideia é captar vibração e ruído, interpretar esses sinais localmente e identificar quando o padrão de funcionamento deixa de corresponder à condição normal conhecida.
>
> A decisão acontece no próprio nó de borda, permitindo alerta e resposta local mesmo quando a conectividade não está disponível.

**Resultado visível esperado:** a banca entende que a solução transforma sinais físicos em informação para apoiar decisões de manutenção.

### 1:00-1:40 - Retomada da demonstração e resultado

**Ação:** manter visível o estado final da bancada ou o último evento registrado no painel.

**Fala sugerida:**

> Na demonstração, acompanhamos o equipamento em condição normal e observamos como uma alteração controlada modifica os sinais captados. A proposta é que o sistema classifique essa mudança, sinalize localmente e registre o evento para acompanhamento remoto.
>
> Quando a demonstração mostrar esses comportamentos, ela evidencia o fluxo completo: monitorar, identificar uma condição anômala, responder localmente e preservar o registro mesmo diante de uma indisponibilidade de rede.

**Resultado visível esperado:** a banca associa os comportamentos demonstrados aos requisitos da PoC, sem confundir uma simulação ou comportamento previsto com uma validação já concluída.

### 1:40-2:05 - Alcance e limites da PoC

**Ação:** exibir os limites no slide final ou mencioná-los enquanto aponta a bancada.

**Fala sugerida:**

> Esta prova de conceito não calcula o tempo exato até uma falha nem substitui os sistemas de segurança de uma planta industrial. O foco é reconhecer padrões de degradação já conhecidos em um equipamento de bancada e fornecer evidências para uma intervenção mais bem orientada.
>
> A evolução para um ambiente industrial exigiria validação em diferentes rotações e condições de operação, maior volume de dados e integração adequada com os sistemas de segurança e manutenção da planta.

**Resultado visível esperado:** a banca entende claramente o valor da solução sem atribuir à PoC capacidades fora de seu escopo.

### 2:05-2:30 - Encerramento

**Ação:** manter visíveis o slide final, a bancada e o painel. Fazer uma breve pausa antes da última frase.

**Fala sugerida:**

> Em síntese, nossa proposta busca transformar vibração e ruído em informação para agir antes que uma alteração de condição se torne uma falha mais grave.
>
> Ao combinar sensoriamento, processamento local e registro de eventos, o monitoramento preditivo vibroacústico cria uma base para uma manutenção menos reativa, menos dependente de calendário e mais orientada pela condição real do equipamento.
>
> Obrigado.

**Resultado visível esperado:** a apresentação encerra retomando problema, solução e resultado esperado de forma coerente e direta.

## Preparação antes da apresentação

- Confirmar quais comportamentos foram realmente demonstrados ao vivo.
- Ajustar a fala da seção “Retomada da demonstração e resultado” conforme as evidências obtidas.
- Manter a bancada e o painel no último estado compreensível para a banca.
- Usar um slide final com pouco texto e sem novos conceitos técnicos.
- Cronometrar a conclusão para permanecer próxima de 2min30s.