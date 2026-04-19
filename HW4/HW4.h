#ifndef HW3_H
#define HW3_H


// I2C defines
// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

#define LEDPIN CYW43_WL_GPIO_LED_PIN

void i2c_init_all();
#endif 