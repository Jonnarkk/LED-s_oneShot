# LED's_oneShot
Este projeto consiste na utilização de um temporizador one-shot (disparo único) para a simulação de um sistema de LED's com um botão.

## Descrição de funcionamento
Após pressionar o botão, todos os LED's são ativados, após 3 segundos, um dos LED's é desativado, após outros 3 segundos, um outro LED é também desativado, o processo se repete até que 
todos os leds estejam apagados. Durante o processo de desligamento dos LED's, o botão não terá efeito e não poderá ativar todos os LED's novamente, ou seja, será **ignorado** o sinal do
botão até que todos os LED's apaguem.

## Estrutura do código
O código está organizado da seguinte maneira:

- `inicializar_pinos()`: Inicializa os pinos necessários.
- `int64_t turn_off_callback(alarm_id_t id, void *user_data)`: Realiza lógica após temporizador one-shot ser configurado.
- `button_callback(uint gpio, uint32_t events)`: Realiza lógica da interrupção quando o botão é pressionado.
- `gpio_set_irq_enabled_with_callback(botao_A, GPIO_IRQ_EDGE_FALL, true, &button_callback)`: Configura interrupção do botão para borda de descida.
- `add_alarm_in_ms(3000, turn_off_callback, NULL, true)`: Configura temporizador one-shot para chamar função `int64_t turn_off_callback(alarm_id_t id, void *user_data)` em 3 segundos.

## Desenvolvedor
Guilherme Miller Gama Cardoso
