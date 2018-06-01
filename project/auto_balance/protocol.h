/*
 * protocol.h
 *
 *  Created on: Apr 23, 2018
 *      Author: Shu Runxuan
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef union float_u_
{
    float f;
    char bytes[sizeof(float)];
} float_u;

typedef union int_u_
{
    int f;
    char bytes[sizeof(float)];
} int_u;


void process_command(uint16_t length, uint8_t* command);

void check_version(void);
void get_ACC_X(void);
void get_ACC_Y(void);
void get_ACC_Z(void);
void get_GYR_X(void);
void get_GYR_Y(void);
void get_GYR_Z(void);
void get_all_data(void);

void get_balance_kp(void);
void get_balance_kd(void);
void get_velocity_kp(void);
void get_velocity_ki(void);
void get_angle(void);

void set_balance_kp(float);
void set_balance_kd(float);
void set_velocity_kp(float);
void set_velocity_ki(float);
void set_speed(float);
void set_angle(float);

float return_balance_kp(void);



#endif /* PROTOCOL_H_ */
