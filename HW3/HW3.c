#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"
#include "HW3.h"

int main()
{
    stdio_init_all();
    i2c_init_all();

    int rc = cyw43_arch_init();
    int led = 1;
    while (true) {

        unsigned char readbuf = readPin(ADDR, GPIO);
        int button  = readbuf & 0x01;
        setPin(ADDR, IODIR, 0x7F);
        if (button){
            setPin(ADDR, OLAT, 0x00);
        }
        else{
            setPin(ADDR, OLAT, 0x80);
        }
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !led);
        led = !led;
        sleep_ms(100);
        
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

void setPin(unsigned char addr, unsigned char reg, unsigned char val){

    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);

}

unsigned char readPin(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf;
    uint8_t buf[1] = {reg};

    i2c_write_blocking(i2c_default, addr, buf, 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, addr, &readbuf, 1, false);  // false - finished with bus

    return readbuf;
}
