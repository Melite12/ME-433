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

    writePin(ADDR, PWR_MGMT_1, 0x00);
    writePin(ADDR, ACCEL_CONFIG, 0x00);
    writePin(ADDR, GYRO_CONFIG, 0b00011000);

    struct Data data;
    while (true) { 
        data = readData(ADDR, ACCEL_XOUT_H);

        printf("Acc_x: %.3f   Acc_y: %.3f   Acc_z: %.3f\n", data.acc_x, data.acc_y, data.acc_z);
        printf("Gyro_x: %.3f   Gryo_y: %.3f   Gyro_z: %.3f\n", data.gyro_x, data.gyro_y, data.gyro_z);
        printf("Temp: %.2f\n", data.temp);

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
        led = !led;
        sleep_ms(1000);
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

void writePin(unsigned char addr, unsigned char reg, unsigned char val){

    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(I2C_PORT_1, addr, buf, 2, false);

}

unsigned char readPin(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf;
    uint8_t buf[1] = {reg};

    i2c_write_blocking(I2C_PORT_1, addr, buf, 1, true);  // true to keep host control of bus
    i2c_read_blocking(I2C_PORT_1, addr, &readbuf, 1, false);  // false - finished with bus

    return readbuf;
}

struct Data readData(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf[14];
    uint8_t buf[1] = {reg};

    int wr = i2c_write_blocking(I2C_PORT_1, addr, buf, 1, true);  // true to keep host control of bus
    int rd = i2c_read_blocking(I2C_PORT_1, addr, readbuf, 14, false);  // false - finished with bus

    if (wr == PICO_ERROR_GENERIC || rd == PICO_ERROR_GENERIC) {
        printf("I2C error! wr=%d rd=%d\n", wr, rd);
        // return empty/last data or handle error
    }

    struct Data data;
    data.acc_x =  (int16_t)((readbuf[0] << 8) | readbuf[1]) * ACC_TO_G ;
    data.acc_y =  (int16_t)((readbuf[2] << 8) | readbuf[3]) * ACC_TO_G;
    data.acc_z =  (int16_t)((readbuf[4] << 8) | readbuf[5]) * ACC_TO_G;
    data.temp =   (int16_t)((readbuf[6] << 8) | readbuf[7]) / 340.0 + 36.53;
    data.gyro_x = (int16_t)((readbuf[8] << 8) | readbuf[9]) * GYRO_TO_DPS;
    data.gyro_y = (int16_t)((readbuf[10] << 8) | readbuf[11]) * GYRO_TO_DPS;
    data.gyro_z = (int16_t)((readbuf[12] << 8) | readbuf[13]) * GYRO_TO_DPS;

    return data;
}
