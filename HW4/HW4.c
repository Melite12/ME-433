#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "HW4.h"
#include "ssd1306.h"
#include "font.h"

#include "pico/cyw43_arch.h"

int main()
{
    
    stdio_init_all();
    i2c_init_all();
    ssd1306_setup();

    int rc = cyw43_arch_init();
    int led = 1;
    unsigned char pixel = 1;
    unsigned char x = 10;
    unsigned char y = 10;

    sleep_ms(2000);

    unsigned int t1;
    unsigned int t2;
    float diff_us;
    float fps;
    
    while (true) {
        // draw Frame
        t1 = to_us_since_boot(get_absolute_time());
        ssd1306_clear();
        
        char message[50]; 
        sprintf(message, "the quick brown fox jumps"); 
        drawString  (0,0,message);
        drawString  (0,8,message);
        drawString  (0,16,message);
        drawString  (0,24,message);

        ssd1306_update();
        t2 = to_us_since_boot(get_absolute_time());

        sleep_ms(500);
        
        // draw FPS
        diff_us = absolute_time_diff_us(t1, t2);
        fps = 1000000.0 / diff_us;
        sprintf(message, "FPS = %.1f", fps);

        ssd1306_clear();
        drawString(0,0,message);
        ssd1306_update();

        // LED Blink
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


