#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define ENCODER_ADDR  0x36
#define REG_ANGLE_H  0x0E

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

void i2c_init_all();
float encoder_read();


int main()
{
    stdio_init_all();
    sleep_ms(100);
    i2c_init_all();
    sleep_ms(100);
    while (true) {
        float angle = encoder_read();
        printf("%f\n", angle);
        sleep_ms(1000);
    }
}

void i2c_init_all(){
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

float encoder_read(){
    uint8_t reg = REG_ANGLE_H;
    uint8_t buf[2];
    uint16_t val = 0;
    
    i2c_write_blocking(i2c0, ENCODER_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, ENCODER_ADDR, buf, 2, false);

    val = ((uint16_t)(buf[0] & 0x0F) << 8) | buf[1];
    float angle_deg = (val / 4096.0f) * 360.0f;

    return angle_deg;

}
