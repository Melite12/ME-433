#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "HW8.h"

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
    ram_init();
    ram_write_sin();

    while (true) {
        for(int i = 0; i < 1024*2; i+=2){
            update_dac_from_ram(i);
            sleep_ms(1);
        }
    }
}

void SPI_init(){

    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS_DAC,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_CS_RAM,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_set_dir(PIN_CS_DAC, GPIO_OUT);
    gpio_put(PIN_CS_DAC, 1);

    gpio_set_dir(PIN_CS_RAM, GPIO_OUT);
    gpio_put(PIN_CS_RAM, 1);
}

void ram_init(){
    int len = 2;
    uint8_t data[len];
    data[0] = 0b00000001;
    data[1] = 0b01000000;
    cs_select(PIN_CS_RAM);
    spi_write_blocking(SPI_PORT, data, len);
    cs_deselect(PIN_CS_RAM);
}

void ram_write_sin(){

    int len = 2;
    uint8_t data[len];
    float voltage;
    uint16_t v_16bit;
    uint16_t config = 0b0011 << 12;
    uint16_t addr = 0;

    for(int i = 0; i < 1024; i++){
        voltage = (sin(2 * M_PI * i / 1024) + 1) * 512;
        v_16bit = ((uint16_t)voltage & 0b111111111111);
        v_16bit = v_16bit << 2;
        v_16bit = v_16bit | config;

        data[0] = v_16bit >> 8;
        data[1] = v_16bit & 0xFF;

        ram_write(addr, data);
        addr += 2;
    }
}

void ram_write(uint16_t addr, uint8_t *data){
    int len = 5;
    uint8_t packet[len];
    packet[0] = 0b00000010;
    packet[1] = addr>>8;
    packet[2] = addr&0xFF;
    packet[3] = data[0];
    packet[4] = data[1];

    cs_select(PIN_CS_RAM);
    spi_write_blocking(SPI_PORT, packet, len);
    cs_deselect(PIN_CS_RAM);
}

void update_dac_from_ram(int i){
    int len = 2;
    uint8_t data[len];
    ram_read(i, data);

    cs_select(PIN_CS_DAC);
    spi_write_blocking(SPI_PORT, data, len);
    cs_deselect(PIN_CS_DAC);
}

void ram_read(uint16_t addr, uint8_t *data){
    int len = 5;
    uint8_t packet[5];
    packet[0] = 0b00000011;
    packet[1] = addr>>8;
    packet[2] = addr & 0xFF;
    packet[3] = 0;
    packet[4] = 0;

    uint8_t dst[5];
    cs_select(PIN_CS_RAM);
    spi_write_read_blocking(SPI_PORT, packet, dst, len);
    cs_deselect(PIN_CS_RAM);
    
    data[0] = dst[3];
    data[1] = dst[4];
}