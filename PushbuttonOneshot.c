#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Defines
#define red_pin 13
#define blue_pin 12
#define green_pin 11
#define botao_A 5

//Variáveis globais
volatile uint8_t leds_active = 0; // Contador para situar qual LED deve apagar
uint32_t last_time = 0; // Variável para realização do debouncing
volatile bool botao_press = false; // Booleano para verificar aperto do botão
bool timer = false; // Booleano para verificar se timer está ativo


// Função para inicializar todos os pinos necessários
void inicializar_pinos(){
    gpio_init(red_pin);
    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_init(blue_pin);
    gpio_set_dir(blue_pin, GPIO_OUT);
    gpio_init(green_pin);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_init(botao_A);
    gpio_set_dir(botao_A, GPIO_IN);
    gpio_pull_up(botao_A);
}

// Função onde o temporizador oneshot realiza sua lógica
int64_t turn_off_callback(alarm_id_t id, void *user_data){
    switch (leds_active){
    case 0:
        gpio_put(red_pin, 0);
        gpio_put(green_pin, 1);
        gpio_put(blue_pin, 1);
        leds_active = 1;
        break;
    case 1:
        gpio_put(red_pin, 0);
        gpio_put(green_pin, 1);
        gpio_put(blue_pin, 0);
        leds_active = 2;
        break;
    case 2:
        gpio_put(red_pin, 0);
        gpio_put(green_pin, 0);
        gpio_put(blue_pin, 0);
        leds_active = 0;
        timer = false;
        return 0;
        break;
    }
    
    return 3000 * 1000;
    
}

// Função da interrupção onde está situada a lógica para aperto do botão
void button_callback(uint gpio, uint32_t events) {
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if(!timer){
        if (gpio == botao_A && (current_time - last_time) > 50) {
            if (leds_active == 0) {
                botao_press = true;
            }
            last_time = current_time;
        }
    }
}

int main(){
    stdio_init_all();
    inicializar_pinos();

    gpio_set_irq_enabled_with_callback(botao_A, GPIO_IRQ_EDGE_FALL, true, &button_callback); // Função que configura a interrupção do botão

    while (true) {
        if (botao_press && leds_active == 0) {
            leds_active = 0;
            botao_press = false;
            timer = true;

            gpio_put(red_pin, 1);
            gpio_put(green_pin, 1);
            gpio_put(blue_pin, 1);

        // Inicia o alarme de 3 segundos
        add_alarm_in_ms(3000, turn_off_callback, NULL, true); // Função de configuração do temporizador oneshot
        }

        sleep_ms(10); // Pausa para diminuir a frequência de operação

    }

    return 0;
}
