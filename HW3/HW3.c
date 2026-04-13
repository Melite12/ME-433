#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"
#include "HW3.h"


// const uint8_t buf1[2] = {0x00, 0x7F};   
// const uint8_t buf2[2] = {0x0A, 0x80};
// const uint8_t buf3[1] = {0x09};
// const uint8_t buf4[2] = {0x0A, 0x00};
uint8_t readbuf;

int main()
{
    stdio_init_all();
    i2c_init_all();

    int rc = cyw43_arch_init();

    while (true) {
        printf("%d\n",readbuf);

        i2c_write_blocking(i2c_default, ADDR, buf3, 1, true);  // true to keep host control of bus
        i2c_read_blocking(i2c_default, ADDR, &readbuf, 1, false);  // false - finished with bus

        int button  = readbuf & 0b00000001;
        if (button == 1){
            i2c_write_blocking(i2c_default, 0b0100000, buf1, 2, false);
            i2c_write_blocking(i2c_default, 0b0100000, buf4, 2, false);
        }
        else{
            i2c_write_blocking(i2c_default, 0b0100000, buf1, 2, false);
            i2c_write_blocking(i2c_default, 0b0100000, buf2, 2, false);
        }
        // LED Blink
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(250);
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        // sleep_ms(250);
    }
}

void i2c_init_all(){

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // Addr = 7 bit address of the chip 
    // buf = array of 8bit data you are sending
    // 2 = length of array (should math buf?)
    // false = write ; true = read
    // first byte of buf = address 
}


