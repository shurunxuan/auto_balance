/*
 * protocol.c
 *
 *  Created on: Apr 23, 2018
 *      Author: Shu Runxuan
 */

#include "protocol.h"
#include "uart.h"
#include "mpu_6050.h"

#include <stdio.h>
#include <math.h>

//#define __DEBUG

void process_command(uint16_t length, uint8_t* command)
{
#ifdef __DEBUG
    char value_a[40];
    int i;
    for (i = 0; i < length; ++i)
    {
        memset(value_a, 0x00, 40);
        sprintf(value_a, "%x ", command[i]);
        send_string_pc(value_a, 40);
    }
    send_char_pc('\n');
#endif

    if (command[0] == 0)
    {
        // Check Version
        check_version();
    }
    else if (command[0] == 1)
    {
        // Get ACC_X
        get_ACC_X();
    }
    else if (command[0] == 2)
    {
        // Get ACC_Y
        get_ACC_Y();
    }
    else if (command[0] == 3)
    {
        // Get ACC_Z
        get_ACC_Z();
    }
    else if (command[0] == 4)
    {
        // Get GYR_X
        get_GYR_X();
    }
    else if (command[0] == 5)
    {
        // Get GYR_Y
        get_GYR_Y();
    }
    else if (command[0] == 6)
    {
        // Get GYR_Z
        get_GYR_Z();
    }
    else if (command[0] == 7)
    {
        // Get All Data
        get_all_data();
    }
    else if (command[0] == 0x11)
    {
        // Get Balance k_p
        get_balance_kp();
    }
    else if (command[0] == 0x12)
    {
        // Get Balance k_d
        get_balance_kd();
    }
    else if (command[0] == 0x13)
    {
        // Get Velocity k_p
        get_velocity_kp();
    }
    else if (command[0] == 0x14)
    {
        // Get Velocity k_i
        get_velocity_ki();
    }
    else if (command[0] == 0x31)
    {
        // Get Angle
        get_angle();
    }
    else if (command[0] == 0x15)
    {
        // Set Balance k_p
        float_u b_kp_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            b_kp_u.bytes[i] = command[i + 1];
        }

        set_balance_kp(b_kp_u.f);
    }
    else if (command[0] == 0x16)
    {
        // Set Balance k_d
        float_u b_kd_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            b_kd_u.bytes[i] = command[i + 1];
        }

        set_balance_kd(b_kd_u.f);
    }
    else if (command[0] == 0x17)
    {
        // Set Velocity k_p
        float_u v_kp_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            v_kp_u.bytes[i] = command[i + 1];
        }

        set_velocity_kp(v_kp_u.f);
    }
    else if (command[0] == 0x18)
    {
        // Set Velocity k_i
        float_u v_ki_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            v_ki_u.bytes[i] = command[i + 1];
        }

        set_velocity_ki(v_ki_u.f);
    }
    else if (command[0] == 0x20)
    {
        // Set Speed
        float_u s_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            s_u.bytes[i] = command[i + 1];
        }

        set_speed(s_u.f);
    }
    else if (command[0] == 0x32)
    {
        // Set angle
        float_u a_u;
        int i;
        for (i = 0; i < sizeof(float); ++i)
        {
            a_u.bytes[i] = command[i + 1];
        }

        set_angle(a_u.f);
    }
    else if (command[0] == 0x21)
    {
        start_left_turn();
    }
    else if (command[0] == 0x22)
    {
        start_right_turn();
    }
    else if (command[0] == 0x23)
    {
        stop_turn();
    }
}

void check_version()
{
    send_char_bt(0);
    send_char_bt(1);
}

void get_ACC_X()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u acc_x;

    acc_x.f = mpu_data.ACC_X;
    //acc_x.f = angleAx;
    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(acc_x.bytes[i]);
    }
}

void get_ACC_Y()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u acc_y;

    acc_y.f = mpu_data.ACC_Y;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(acc_y.bytes[i]);
    }
}

void get_ACC_Z()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u acc_z;

    acc_z.f = mpu_data.ACC_Z;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(acc_z.bytes[i]);
    }
}

void get_GYR_X()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u gyr_x;

    gyr_x.f = mpu_data.GYR_X;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(gyr_x.bytes[i]);
    }
}

void get_GYR_Y()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u gyr_y;

    gyr_y.f = mpu_data.GYR_Y;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(gyr_y.bytes[i]);
    }
}

void get_GYR_Z()
{
    int i;
    MPU_DATA mpu_data;
    mpu_data = get_stored_mpu_data();
    float_u gyr_z;

    gyr_z.f = mpu_data.GYR_Z;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(gyr_z.bytes[i]);
    }
}

void get_all_data()
{
    int i, j;
    MPU_DATA mpu_data;
//    float angleAx;
    mpu_data = get_stored_mpu_data();
    float_u data[6];

//#define PI 3.1415926f
//    angleAx=atan2f(mpu_data.ACC_Z,mpu_data.ACC_Y)*180/PI;

    data[0].f = mpu_data.ACC_X;
    data[1].f = mpu_data.ACC_Y;
    data[2].f = mpu_data.ACC_Z;
    data[3].f = mpu_data.GYR_X;
    data[4].f = mpu_data.GYR_Y;
    data[5].f = mpu_data.GYR_Z;

//    data[0].f = angleAx;
//    data[1].f = 0;
//    data[2].f = 0;
//    data[3].f = 0;
//    data[4].f = 0;
//    data[5].f = 0;

    for (j = 0; j < 6; ++j)
    {
        for (i = 0; i < sizeof(float); ++i)
        {
            send_char_bt(data[j].bytes[i]);
        }
    }
}


