/*
 * main_prog.c
 *
 *  Created on: 2018/01/12
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This is the main source file of the program.
 *
 */

#include "main_prog.h"
#include "PID.h"
#include <math.h>

/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "mpu_6050.h"
#include "uart.h"
#include "motor.h"
//#include "protocol.h"
#include "filter.h"

MPU_DATA mpu_data;
float angleAx,angleAy,angleAz;
float AX1,AX2,AX3;
#define PI 3.1415926f
#define MAX_SPEED 1000.0f
//float AX1;

int k = 0;

float dt = 0.00095;

void setup(void)
{
    setup_uart();
    send_string_pc("OK\n", 4);
//    setup_bluetooth();


//    setup_i2c();
//
//    setup_mpu_6050();
    //setup_ratio(ACC_4G, GYR_1000);

    setup_pwm();

    set_motor_1_forward();
    set_motor_2_forward();
    stop_motor();

    setup_i2c();

    //for (k = 0; k < 1000; ++k);

    setup_mpu_6050();
//    send_string_pc("Complete\n", 10);
}

// Currently, this is a test function.
void loop(void)
{
    float angleAx, AX1;
//    char value_a[100];


    int enleft=0,enright=0;
    enleft=get_motor_1_speed();
    enright=-get_motor_2_speed();

    int PWM=0;
    int PWMTEM=0;
    int PWM1=0;
    int PWM2=0;
    int PWM_LEFT = 0;
    int PWM_RIGHT = 0;

    request_mpu_data(&mpu_data);
    angleAx=atan2f(mpu_data.ACC_Z,mpu_data.ACC_Y)*180/PI;
    AX1=Kalman_Filter(angleAx,-mpu_data.GYR_X,dt);
    if (AX1 > 50 || AX1 < -50)
        stop_motor();
    else
        start_motor();
    PWM=balance(AX1,-mpu_data.GYR_X);
    PWM1=velocity(enleft,enright);
    PWM2=turn(mpu_data.GYR_Y);
    PWMTEM=PWM+PWM1;
    PWM_LEFT = PWMTEM - PWM2;
    PWM_RIGHT = PWMTEM + PWM2;

    PWM_LEFT=PWM_LEFT/15000.0f*1000.0f;
    PWM_LEFT = PWM_LEFT > MAX_SPEED ? MAX_SPEED : PWM_LEFT;
    PWM_LEFT = PWM_LEFT < -MAX_SPEED ? -MAX_SPEED : PWM_LEFT;
    PWM_RIGHT=PWM_RIGHT/15000.0f*1000.0f;
    PWM_RIGHT = PWM_RIGHT > MAX_SPEED ? MAX_SPEED : PWM_RIGHT;
    PWM_RIGHT = PWM_RIGHT < -MAX_SPEED ? -MAX_SPEED : PWM_RIGHT;

    set_motor_1_direction(PWM_LEFT>0);
    set_motor_2_direction(PWM_RIGHT<0);
    set_pwm_duty(abs(PWM_LEFT),abs(PWM_RIGHT));

}
