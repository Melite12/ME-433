#include <stdio.h>
#include <math.h>
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
    float time = 0;
    float freq = 2.0;
    uint8_t data[2];
    size_t len = 2;

    stdio_init_all();
    SPI_init();
    adc_init_all();    


    while (true) {
        // calculate sin
        sin_math(time, data);

        cs_select(PIN_CS);
        spi_write_blocking(SPI_PORT, data, len); // where data is a uint8_t array with length len
        cs_deselect(PIN_CS);

        // calculate triangle
        triangle_math(time, data);

        cs_select(PIN_CS);
        spi_write_blocking(SPI_PORT, data, len); // where data is a uint8_t array with length len
        cs_deselect(PIN_CS);

        // adc
        int val = adc_read();
        
        sleep_ms(1);
        
        time = time + 0.001;
        if (time >= 1.0){
            time = 0;
        }
    }
}

void SPI_init(){
    // SPI initialisation
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
}

void adc_init_all(){
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);
}

void sin_math(float time, uint8_t *data){

    float sin_val = sin(2 * (time*2*M_PI));
    uint16_t val_10bit = (uint16_t)((sin_val +1.0) * 511.5);

    val_10bit = val_10bit << 2;
    uint16_t config = 0b0011 << 12;
    uint16_t final = config | val_10bit;

    data[0] = final >> 8;
    data[1] = final & 0xff;

}

void triangle_math(float time, uint8_t *data){
    float val;
    if (time <= 0.5){
        val = (time/0.5) * 1023.0;
    }
    else{
        val = 1023.0 - (((time - 0.5)/0.5) * 1023.0);
    }

    uint16_t val_10bit = (uint16_t)(val);
    val_10bit = val_10bit << 2;
    uint16_t config = 0b1011 << 12;
    uint16_t final = config | val_10bit;

    data[0] = final >> 8;
    data[1] = final & 0xff;
}
