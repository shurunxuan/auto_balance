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

MPU_DATA mpu_data;

// Convert integers to strings
char* itoa(int num, char* str, int radix);

int duty = 0;
int direction = 1;

void setup(void)
{
    setup_uart();

    setup_i2c();

    setup_mpu_6050();
    //setup_ratio(ACC_4G, GYR_1000);

    setup_pwm();

    stop_motor();
    set_motor_1_forward();
    set_motor_2_forward();

    send_string("Complete", 9);
}

// Currently, this is a test function.
void loop(void)
{
    duty += direction * 2;
    if (duty >= TIMER_PERIOD)
    {
        duty = TIMER_PERIOD;
        direction = -1;
    }
    else if (duty <= 0)
    {
        duty = 0;
        direction = 1;
    }
    set_pwm_duty(duty, TIMER_PERIOD - duty);

    set_motor_run(mpu_data.ACC_Z > -1.2f);
    set_motor_1_direction(mpu_data.ACC_Y < -1.2f);
    set_motor_2_direction(mpu_data.ACC_Y > 1.2f);

    request_mpu_data(&mpu_data);
    char value_a[10];
    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.ACC_X);
    send_string(value_a, 10);

    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.ACC_Y);
    send_string(value_a, 10);

    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.ACC_Z);
    send_string(value_a, 10);

//    memset(value_a, 0x00, 10);
//    itoa(mpu_data.TEMP, value_a, 10);
//    send_string(value_a, 10);
//    send_string("\t", 2);

    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.GYR_X);
    send_string(value_a, 10);

    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.GYR_Y);
    send_string(value_a, 10);

    memset(value_a, 0x00, 10);
    sprintf(value_a, "%f\t", mpu_data.GYR_Z);
    send_string(value_a, 10);

    memset(value_a, 0x00, 10);
    itoa(get_motor_1_speed(), value_a, 10);
    send_string(value_a, 10);
    send_string("\t", 2);

    memset(value_a, 0x00, 10);
    itoa(get_motor_2_speed(), value_a, 10);
    send_string(value_a, 10);
    send_string("\n", 2);
}

char* itoa(int num, char* str, int radix)
{
    char index[] = "0123456789ABCDEF";
    unsigned unum;
    int i = 0, j, k;

    if (radix == 10 && num < 0)
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num;
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';

    if (str[0] == '-')
        k = 1;
    else
        k = 0;
    char temp;
    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}
