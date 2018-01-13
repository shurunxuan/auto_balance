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

// Raw data struct of MPU-6050
typedef struct MPU_DATA_RAW_
{
    int16_t ACC_X;
    int16_t ACC_Y;
    int16_t ACC_Z;
    int16_t TEMP;
    int16_t GYR_X;
    int16_t GYR_Y;
    int16_t GYR_Z;
} MPU_DATA_RAW;

void setup_i2c(void);
void setup_mpu_6050(void);
void request_raw_mpu_data(MPU_DATA_RAW* data);



#endif
