#include <stdio.h>
#include "pico/stdlib.h"
#include "HW14.h"


int main()
{
    stdio_init_all();

    hx711_init();

    while (true) {
        int num;
        scanf("%d",&num);

        int voltages[5000];
        uint64_t times[5000];
        
        for (int i = 0; i < num; i++){
            int val = hx711_read();

            voltages[i] = val;
            times[i] = to_ms_since_boot(get_absolute_time());
        }

        for (int i = 0; i < num; i++){
            printf("%d %llu %d\n", i, times[i], voltages[i]);
        }

        sleep_ms(100);
    }
}

void hx711_init(){
    // Not too sure why there's a pull up for PIN_DOUT (does it stay pulled up)

    gpio_init(PIN_DOUT);
    gpio_set_dir(PIN_DOUT, GPIO_IN);
    gpio_pull_up(PIN_DOUT);

    gpio_init(PIN_SCK);
    gpio_set_dir(PIN_SCK, GPIO_OUT);
    gpio_put(PIN_SCK, 0);
}

int hx711_read(){

    uint64_t clock_time_us = 10;
    unsigned int data;

    while(gpio_get(PIN_DOUT)){
        tight_loop_contents();
    }

    unsigned int raw = 0;
    for (int i = 0; i < 24; i++){
        gpio_put(PIN_SCK, 1);
        sleep_us(clock_time_us);

        if (gpio_get(PIN_DOUT)){
            data = 1;
        }
        else{
            data = 0;
        }

        raw = (raw << 1) | data;

        gpio_put(PIN_SCK, 0);
        sleep_us(clock_time_us);
    }

    // set gain for next piece of data
    gpio_put(PIN_SCK, 1);
    sleep_us(clock_time_us);
    gpio_put(PIN_SCK, 0);
    sleep_us(clock_time_us);

    if (raw & 0x800000){
        raw = raw | 0xFF000000;
    }

    return (int)raw;
}
