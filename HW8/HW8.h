#ifndef HW8_H
#define HW8_H

#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS_DAC 20
#define PIN_CS_RAM 17
#define PIN_SCK  18
#define PIN_MOSI 19

void SPI_init();
void ram_init();
void ram_write_sin();
void ram_write(uint16_t addr, uint8_t *data);
void ram_read(uint16_t addr, uint8_t *data);
void update_dac_from_ram(int i);

#endif 