#ifndef HW7_J
#define HW7_H

// SPI Defines
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19


void SPI_init();
void adc_init_all();
void sin_math(float time, uint8_t *data);
void triangle_math(float time, uint8_t *data);

#endif