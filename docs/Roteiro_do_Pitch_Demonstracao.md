# Roteiro do Pitch - Demonstração

## Monitoramento Preditivo Vibroacústico com Edge AI

**Duração prevista:** 6 minutos

**Formato:** demonstração prática e narrada da prova de conceito

**Objetivo:** mostrar como o protótipo identifica uma mudança nos padrões de vibração e ruído de um equipamento rotativo, reage localmente e mantém o registro e a comunicação dos eventos.

## Organização do espaço

A demonstração utiliza três áreas visíveis para a banca:

1. **Bancada do protótipo:** cooler que representa o equipamento rotativo, sensores, ESP32-S3, buzzer, relé, RTC e MicroSD. O cooler deve estar protegido e preso à bancada.
2. **Tela de monitoramento:** computador com o painel aberto, mostrando o estado recebido via MQTT e os eventos detectados.
3. **Área de apresentação:** o apresentador fica ao lado da bancada, sem bloquear a visão do protótipo. Um segundo integrante pode operar o painel e controlar a conexão de rede. Se houver apenas um apresentador, o painel deve ficar ao alcance dele.

Na bancada, os estados **NORMAL** e **ANOMALIA** devem estar identificados de forma legível. O mecanismo usado para provocar o desbalanceamento precisa estar preparado antes do pitch e permitir uma alteração rápida, segura e reversível.

## Roteiro detalhado

### 0:00-0:30 - Gancho vindo da solução

**Ação:** o cooler permanece desligado. O apresentador indica, sem descrever individualmente todos os componentes, o caminho entre equipamento, sensores, ESP32-S3, resposta local e painel.

**Fala sugerida:**

> Acabamos de mostrar que nossa solução transforma vibração e ruído em dados e analisa esses sinais diretamente na borda. Agora vamos acompanhar esse processo acontecendo na prática: primeiro com o equipamento em condição normal e, em seguida, com uma anomalia controlada.

**Resultado visível esperado:** a banca reconhece o equipamento monitorado, o nó de processamento e o painel que será observado durante a demonstração.

### 0:30-1:15 - Funcionamento normal

**Ação:** ligar o cooler em sua configuração balanceada. Apontar o funcionamento contínuo dos sensores e mostrar no painel o estado normal recebido pelo sistema.

**Fala sugerida:**

> Neste primeiro momento, o cooler representa um equipamento operando normalmente. Os sensores captam continuamente sua vibração e seu padrão acústico. O ESP32-S3 processa as janelas de dados e executa a classificação localmente. Como o padrão observado corresponde à condição saudável conhecida pelo modelo, o sistema mantém o estado normal, sem acionar o alerta ou interromper o equipamento.

**Resultado visível esperado:** cooler em funcionamento, estado **NORMAL** no painel, buzzer inativo e relé mantendo a alimentação do equipamento.

### 1:15-2:00 - Introdução de uma anomalia controlada

**Ação:** desligar o cooler brevemente, aplicar o mecanismo de desbalanceamento previamente preparado e religá-lo. Não tocar nas pás enquanto estiverem em movimento.

**Fala sugerida:**

> Agora vamos introduzir um desbalanceamento controlado. Ele simula uma alteração mecânica capaz de modificar a assinatura de vibração e ruído do equipamento. Para uma pessoa, essa mudança pode ser sutil ou percebida tarde demais; para o sistema, ela passa a ser uma variação mensurável.

**Resultado visível esperado:** cooler operando na condição desbalanceada, com alteração nos sinais captados pelos sensores.

### 2:00-3:10 - Inferência e alerta local

**Ação:** acompanhar a saída do sistema e indicar a mudança da classificação. Assim que a anomalia for identificada, mostrar o alerta local e a atualização do painel.

**Fala sugerida:**

> Os novos dados são convertidos em características que representam o comportamento atual do equipamento. A inferência continua acontecendo no próprio ESP32-S3, sem depender de uma resposta da nuvem. Quando a janela analisada corresponde a um padrão anômalo conhecido, o estado muda para anomalia e o buzzer sinaliza imediatamente a necessidade de atenção.
>
> Essa classificação não informa uma data exata para a falha. Ela indica que o equipamento saiu do padrão saudável conhecido, permitindo que a equipe de manutenção investigue a causa antes de uma ocorrência mais grave.

**Resultado visível esperado:** classificação **ANOMALIA**, buzzer acionado e evento atualizado no painel.

### 3:10-4:00 - Persistência da anomalia e atuação do relé

**Ação:** manter a condição anômala durante três classificações consecutivas. Mostrar o acionamento do relé e a interrupção do cooler.

**Fala sugerida:**

> Uma leitura isolada pode sofrer influência momentânea. Por isso, o corte não ocorre no primeiro alerta. Quando a condição anômala persiste por três classificações consecutivas, o sistema aciona o relé e interrompe a alimentação do equipamento de teste. Nesta prova de conceito usamos uma carga de baixa tensão; em uma aplicação industrial real, a atuação exigiria integração com os sistemas de segurança da planta.

**Resultado visível esperado:** relé acionado e cooler desligado após a confirmação da anomalia persistente.

### 4:00-5:10 - Operação independente da rede

**Ação:** restaurar a condição balanceada e reiniciar o monitoramento. Desconectar o Wi-Fi de teste. Provocar novamente a condição anômala e mostrar que a aquisição, a inferência, o alerta local e o registro continuam funcionando.

**Fala sugerida:**

> Como a conectividade pode ser instável em um ambiente industrial, a função central do protótipo não depende da rede. Vamos interromper o Wi-Fi e repetir a alteração. Mesmo sem comunicação com o painel, os sensores continuam sendo lidos, a decisão continua sendo tomada localmente e o alerta permanece ativo. O evento recebe uma referência de tempo do RTC e fica armazenado no MicroSD para envio posterior.

**Resultado visível esperado:** painel sem receber novas mensagens durante a desconexão, enquanto o protótipo continua detectando e sinalizando localmente a anomalia. O registro local deve ser evidenciado pela saída de diagnóstico ou pela leitura preparada do log.

### 5:10-5:40 - Reconexão e atualização remota

**Ação:** restabelecer o Wi-Fi e mostrar o reenvio cronológico do evento registrado durante a desconexão.

**Fala sugerida:**

> Ao restabelecer a conexão, o sistema retoma a comunicação e envia os eventos que ficaram armazenados, preservando sua ordem e referência temporal. Dessa forma, uma falha momentânea de rede não apaga o histórico nem impede a resposta local.

**Resultado visível esperado:** reconexão MQTT e exibição, no painel, do evento produzido enquanto o sistema estava offline.

### 5:40-6:00 - Ponte para a conclusão

**Ação:** manter na tela o último evento e indicar o conjunto completo da bancada.

**Fala sugerida:**

> Vimos o equipamento sair da condição normal, o sistema reconhecer a mudança, alertar localmente, atuar diante da persistência e preservar o evento mesmo sem rede. Com isso, a manutenção deixa de depender apenas da percepção humana ou de um calendário fixo e passa a receber evidências sobre a condição real do equipamento. A partir desse resultado, podemos concluir o impacto e os limites desta prova de conceito.

## Preparação antes da apresentação

- Fixar o cooler e proteger suas pás, cabos e conexões.
- Utilizar somente uma carga de baixa tensão na demonstração do relé.
- Testar previamente as condições normal e anômala na mesma montagem e posição que serão usadas no pitch.
- Deixar o mecanismo de desbalanceamento pronto, identificado e ao alcance do operador.
- Carregar no ESP32-S3 a versão validada do firmware e do modelo.
- Confirmar o funcionamento do BNO085, KY-038, ADS1115, RTC, MicroSD, buzzer e relé.
- Conferir a conexão Wi-Fi, o broker MQTT e a atualização do painel.
- Abrir antecipadamente o painel e a saída de diagnóstico ou log usada como evidência.
- Limpar eventos de testes anteriores para evitar confusão durante a demonstração.
- Ensaiar com cronômetro, usando as transições de tempo deste roteiro.
- Manter o equipamento reserva, os cabos necessários e a gravação de contingência acessíveis, sem ocupar a área principal da bancada.

## Plano de contingência

A prioridade é preservar a sequência narrativa e mostrar ao vivo tudo o que continuar operacional.

- **Se a anomalia não for classificada:** repetir uma única vez o procedimento validado. Se a segunda tentativa falhar, mostrar a captura previamente registrada da inferência e explicar objetivamente o resultado esperado, sem atribuir ao teste ao vivo um resultado que não ocorreu.
- **Se o painel ou MQTT falhar:** continuar mostrando a inferência, o buzzer, o relé e o registro local. Em seguida, utilizar uma captura ou gravação curta do painel recebendo o mesmo tipo de evento.
- **Se a rede não puder ser desconectada ou restabelecida:** mostrar um log previamente produzido com os horários de armazenamento e reenvio, explicando que essa evidência corresponde ao ensaio de perda de conectividade.
- **Se o relé não atuar:** desligar o cooler manualmente por segurança, informar que a atuação esperada não ocorreu naquele ensaio e usar uma gravação curta da validação anterior.
- **Se o sensor ou o ESP32-S3 parar de responder:** não gastar o tempo do pitch refazendo conexões. Passar para a gravação integral de contingência e narrá-la com a mesma ordem deste roteiro.

A gravação de contingência deve mostrar a bancada e o painel simultaneamente, possuir boa legibilidade e reproduzir toda a sequência prática em menos de 6 minutos. Ela é um recurso de emergência e não substitui o ensaio da demonstração ao vivo.
