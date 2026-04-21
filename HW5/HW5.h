#ifndef HW5_H
#define HW5_H

// i2c for screen
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

//i2c for imu
#define I2C_PORT_1 i2c1
#define I2C_SDA_1 14
#define I2C_SCL_1 15

// config / misc
#define ADDR 0x68
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define WHO_AM_I 0x75

// sensor data registers
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48

// Conversion
#define ACC_TO_G 0.000061
#define GYRO_TO_DPS 0.007630

struct Data{
    float acc_x;
    float acc_y;
    float acc_z;
    float temp;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};

void i2c_init_all();
unsigned char readPin(unsigned char addr, unsigned char reg);
void writePin(unsigned char addr, unsigned char reg, unsigned char val);
struct Data readData(unsigned char addr, unsigned char reg);

#endif