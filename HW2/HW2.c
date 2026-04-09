#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWMPIN 16

void setServo(int angle);

int main()
{
    stdio_init_all();

    gpio_set_function(PWMPIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWMPIN);
    float div = 50;
    pwm_set_clkdiv(slice_num, div);
    uint16_t wrap = 60000;

    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(PWMPIN, 0);

    while (true) {
        int i = 0; 
        for(i = 0; i < 150; i++){
            setServo(i);
            sleep_ms(10);
        }
        for(i = 150; i > 100; i--){
            setServo(i);
            sleep_ms(10);
        }
    }
}

void setServo(int angle){
    pwm_set_gpio_level(PWMPIN, (int)((0.04 + (angle/180)*0.11)*60000));
}