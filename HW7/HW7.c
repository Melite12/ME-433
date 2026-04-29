#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "HW7.h"
#include "hardware/adc.h"

static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}


int main()
{
    stdio_init_all();
    SPI_init();
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);

    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
    
    // A'/B ; BUF; Gain'; SHDN'; 10-bit; 00 
    uint8_t data[2] = {0b00111000, 0b11000000};
    size_t len = 2;
    
    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, len); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);


    while (true) {
        int val = adc_read();
        printf("%d\n",val);
        sleep_ms(1000);
    }
}

void SPI_init(){
    // SPI initialisation
    spi_init(SPI_PORT, 12*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
}

