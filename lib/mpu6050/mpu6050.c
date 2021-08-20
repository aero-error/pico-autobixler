/**
 * This is a driver for the MPU6050 Inertial Motion Unit
 * (IMU) for the Raspberry Pi Pico using the I2C Protocol.
 * 
 * AUTHOR: Michael Gromski
 * 
 */

#include "mpu6050.h"

struct imu_data
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    float temp;
} raw_imu_data;

static void mpu6050_reset(i2c_inst_t *i2c_bus) 
{
    
    // Clear registers
    uint8_t send_buf[] = {PWR_MGMT_1, DEVICE_RESET};
    i2c_write_blocking(i2c_bus, MPU6050_ADDR, send_buf, 2, false);

    // verify device ID
    send_buf[1] = WHO_AM_I;


}

static void mpu6050_read_raw()
{

}
