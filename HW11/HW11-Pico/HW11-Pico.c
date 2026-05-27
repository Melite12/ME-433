#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5


void uart_init_all();

int main()
{
    stdio_init_all();
    uart_init_all();
    
    while (true) {
        
        int c = getchar_timeout_us(0);  // non-blocking
        if (c != PICO_ERROR_TIMEOUT) {
            if ((char)c == '\r') {
                uart_putc(UART_ID, '\n');  // convert to \n for STM32
            } else {
                uart_putc(UART_ID, (char)c);
            }
        }

        while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);
        if (c == '\n') {
            putchar('\n');
        } else {
            putchar(c);
        }
}

        sleep_ms(10);
    }
}

void uart_init_all(){

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // In a default system, printf will also output via the default UART
    sleep_ms(1000);
    // Send out a string, with CR/LF conversions
}
