#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "pio_matrix.pio.h"
#include "numeros.h"

// Lista de todas as matrizes numéricas
double* matrizesNumeros[] = {
    numeroZero, numeroUm, numeroDois, numeroTres, numeroQuatro,
    numeroCinco, numeroSeis, numeroSete, numeroOito, numeroNove
};

// Configurações globais
#define NUM_PIXELS 25
#define OUT_PIN 7
#define TOTAL_NUMEROS 10
#define DEBOUNCE_MS 250
#define LED_PIN_RED 13

// estado do led
bool led_state = false;
static uint32_t ultimo_toggle = 0;

// Botões
const uint BTN_DEC = 6;  // GPIO5 - Decremento
const uint BTN_INC = 5;  // GPIO6 - Incremento

// Variável compartilhada
volatile int numero_atual = 0;
volatile uint32_t ultimo_evento = 0;

// Função de interrupção unificada
void btn_handler(uint gpio, uint32_t events) {
    if((to_ms_since_boot(get_absolute_time()) - ultimo_evento) > DEBOUNCE_MS) {
        if (gpio == BTN_INC) {
            // Incrementa o número
            numero_atual = (numero_atual + 1) % TOTAL_NUMEROS;
            printf("Incremento: %d\n", numero_atual);
        } else if (gpio == BTN_DEC) {
            // Decrementa o número
            numero_atual = (numero_atual - 1 + TOTAL_NUMEROS) % TOTAL_NUMEROS;
            printf("Decremento: %d\n", numero_atual);
        }
        ultimo_evento = to_ms_since_boot(get_absolute_time());
    }
}

// Rotinas de LED
uint32_t matrix_rgb(double b, double r, double g) {
    return ((uint8_t)(g * 255) << 24) | 
           ((uint8_t)(r * 255) << 16) | 
           ((uint8_t)(b * 255) << 8);
}

void desenho_pio(double *desenho, PIO pio, uint sm) {
    for(int i = 0; i < NUM_PIXELS; i++) {
        uint32_t valor = matrix_rgb(desenho[24-i], 0.0, 0.0);
        pio_sm_put_blocking(pio, sm, valor);
    }
}

int main() {
    // Inicialização padrão
    stdio_init_all();
    set_sys_clock_khz(128000, false);

    // Configuração do LED vermelho
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, false);

    // Configuração PIO
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Configuração dos botões
    gpio_init(BTN_INC);
    gpio_init(BTN_DEC);
    gpio_set_dir(BTN_INC, GPIO_IN);
    gpio_set_dir(BTN_DEC, GPIO_IN);
    gpio_pull_up(BTN_INC);
    gpio_pull_up(BTN_DEC);

    // Configuração de interrupções
    gpio_set_irq_enabled_with_callback(BTN_INC, GPIO_IRQ_EDGE_FALL, true, &btn_handler);
    gpio_set_irq_enabled_with_callback(BTN_DEC, GPIO_IRQ_EDGE_FALL, true, &btn_handler);

    while(true) {
        // Controle do LED vermelho (5Hz)
        uint32_t agora = to_ms_since_boot(get_absolute_time());
        if(agora - ultimo_toggle >= 100) { // 100ms = metade do período para 5Hz
            led_state = !led_state;
            gpio_put(LED_PIN_RED, led_state);
            ultimo_toggle = agora;
        }

        // Atualização da matriz de LEDs
        desenho_pio(matrizesNumeros[numero_atual], pio, sm);
        
        sleep_ms(10); // Ajuste fino para precisão temporal
        printf("\nfrequência de clock %ld\r\n", clock_get_hz(clk_sys));
    }
}