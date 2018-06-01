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
    char value_a[100];


    static int enleft=0,enright=0;
    enleft=get_motor_1_speed();
    enright=-get_motor_2_speed();

    static int PWM=0;
    static int PWMTEM=0;
    static int PWM1=0;

    //
    //AX1=Kalman_Filter(angleAx,-mpu_data.GYR_X);
    //request_mpu_data_internal();
    request_mpu_data(&mpu_data);
    //angleAx=atan2f(mpu_data.ACC_Z,mpu_data.ACC_Y)*180/PI;

    angleAx=atan2f(mpu_data.ACC_Z,mpu_data.ACC_Y)*180/PI;
    AX1=Kalman_Filter(angleAx,-mpu_data.GYR_X,dt);

    if (AX1 > 50 || AX1 < -50)
        stop_motor();
    else
        start_motor();

//    memset(value_a, 0x00, 100);
//    sprintf(value_a, "%d\t%d\n",enleft,enright);
//    send_string_bt(value_a,100);

    PWM=balance(AX1,-mpu_data.GYR_X);
    PWM1=velocity(enleft,enright);
//    PWMTEM=PWM;
//    PWMTEM=-PWM1;
    PWMTEM=PWM+PWM1;
//    PWMTEM = PWM;
//    memset(value_a, 0x00, 100);
//    sprintf(value_a, "%d\n",PWMTEM);
//    send_string_bt(value_a,100);

    PWMTEM=PWMTEM/15000.0f*1000.0f;
//    PWMTEM = PWMTEM > 1000.0f ? 1000.0f : PWMTEM;
//    PWMTEM = PWMTEM < -1000.0f ? -1000.0f : PWMTEM;
    PWMTEM = PWMTEM > MAX_SPEED ? MAX_SPEED : PWMTEM;
    PWMTEM = PWMTEM < -MAX_SPEED ? -MAX_SPEED : PWMTEM;
    //if (PWMTEM < 75 && PWMTEM > 0) PWMTEM = 75;
    //if (PWMTEM > -75 && PWMTEM < 0) PWMTEM = -75;

    set_motor_1_direction(PWMTEM>0);
    set_motor_2_direction(PWMTEM<0);
    set_pwm_duty(abs(PWMTEM),abs(PWMTEM));

}
