/*
 * mpu_6050.h
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This header file includes the definitions
 *  of MPU-6050 relative functions and types.
 *         The MPU-6050 passes the data through I2C
 *  Bus, and the data contains 7 values, as defined
 *  in struct MPU_DATA_RAW. The data is currently
 *  raw, which means they aren't floats, but ints.
 *
 */

#ifndef ___MPU_6050_H
#define ___MPU_6050_H

// DriverLib Defines
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Standard Defines
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Actual data struct of MPU-6050
typedef struct MPU_DATA_
{
    float ACC_X;
    float ACC_Y;
    float ACC_Z;
    float TEMP;     // Not processed!
    float GYR_X;
    float GYR_Y;
    float GYR_Z;
} MPU_DATA;

// Acceleration config
typedef enum MPU_ACC_CONF_
{
    ACC_2G  = 0,
    ACC_4G  = 1,
    ACC_8G  = 2,
    ACC_16G = 3
} MPU_ACC_CONF;

// Gyro config
typedef enum MPU_GYR_CONF_
{
    GYR_250  = 0,
    GYR_500  = 1,
    GYR_1000 = 2,
    GYR_2000 = 3
} MPU_GYR_CONF;

void setup_i2c(void);
void i2c_read(uint8_t reg_addr);
void setup_mpu_6050(void);
void setup_ratio(MPU_ACC_CONF acc_conf, MPU_GYR_CONF gyr_conf);
void request_mpu_data(MPU_DATA* data);



#endif
