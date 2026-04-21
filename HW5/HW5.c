#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "HW5.h"
#include "pico/cyw43_arch.h"


int main()
{
    stdio_init_all();
    i2c_init_all();
    int rc = cyw43_arch_init();
    int led = 1;

    sleep_ms(3000);
    unsigned char testing = readPin(ADDR, WHO_AM_I);
    printf("%d",testing);
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
        led = !led;
        sleep_ms(500);
    }
}

void i2c_init_all(){

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    i2c_init(I2C_PORT_1, 400*1000);
    
    gpio_set_function(I2C_SDA_1, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_1, GPIO_FUNC_I2C);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // Addr = 7 bit address of the chip 
    // buf = array of 8bit data you are sending
    // 2 = length of array (should math buf?)
    // false = write ; true = read
    // first byte of buf = address 
}

unsigned char readPin(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf;
    uint8_t buf[1] = {reg};

    i2c_write_blocking(I2C_PORT_1, addr, buf, 1, true);  // true to keep host control of bus
    i2c_read_blocking(I2C_PORT_1, addr, &readbuf, 1, false);  // false - finished with bus

    return readbuf;
}
