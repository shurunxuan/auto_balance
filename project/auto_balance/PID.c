/*
 * PID.c
 *
 *  Created on: 2018/03/27
 *      Author: Nseven
 */


#include "PID.h"
#include "protocol.h"
#include "uart.h"


//float balance_k_p = 3400, balance_k_d = 30;
//float balance_k_p = 4000, balance_k_d = 35;
float balance_k_p = 1800, balance_k_d = 20;
float velocity_k_p = 200, velocity_k_i = 0.1;
float balance_angle = -7.0f;
float speed = 0.0f;

float return_balance_kp(void)
{
    return balance_k_p;
}

void get_balance_kp()
{
    int i;
    float_u b_kp_u;

    b_kp_u.f = balance_k_p;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(b_kp_u.bytes[i]);
    }
}
void get_balance_kd()
{
    int i;
    float_u b_kd_u;

    b_kd_u.f = balance_k_d;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(b_kd_u.bytes[i]);
    }
}
void get_velocity_kp()
{
    int i;
    float_u v_kp_u;

    v_kp_u.f = velocity_k_p;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(v_kp_u.bytes[i]);
    }
}
void get_velocity_ki()
{
    int i;
    float_u v_ki_u;

    v_ki_u.f = velocity_k_i;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(v_ki_u.bytes[i]);
    }
}

void get_angle()
{
    int i;
    float_u a_u;

    a_u.f = balance_angle;

    for (i = 0; i < sizeof(float); ++i)
    {
        send_char_bt(a_u.bytes[i]);
    }
}

void set_balance_kp(float b_kp)
{
    balance_k_p = b_kp;
}
void set_balance_kd(float b_kd)
{
    balance_k_d = b_kd;
}
void set_velocity_kp(float v_kp)
{
    velocity_k_p = v_kp;
}
void set_velocity_ki(float v_ki)
{
    velocity_k_i = v_ki;
}

void set_speed(float s)
{
    speed = s;
}

void set_angle(float a)
{
    balance_angle = a;
}

int balance(float Angle , float Gyro)
{
    float Bias;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ;
    int balance;
    Bias = Angle - balance_angle;
//    if (abs(Bias) < 5) balance_k_p = 600;
//    else  balance_k_p=2000;
    balance = balance_k_p * Bias + Gyro * balance_k_d;
    return balance;
}

int velocity(int encoder_left, int encoder_right)
{
    static float Velocity, Encoder_Least, Encoder;
    static float Encoder_Integral;
    Encoder_Least = (encoder_left + encoder_right) - speed;
//    Encoder_Least = (encoder_left + encoder_right) - 0;
    Encoder *= 0.8;
    Encoder += Encoder_Least * 0.2;
    Encoder_Integral += Encoder;
    if(Encoder_Integral > 100000) Encoder_Integral = 100000;
    if(Encoder_Integral < -100000) Encoder_Integral = -100000;
    Velocity = Encoder * velocity_k_p + Encoder_Integral * velocity_k_i;
    return Velocity;
}

int turn(int encoder_left, int encoder_right, float gyro)
{
    float Turn, Kp = 1, Bias;
    Bias = gyro - 0;
    Turn = Bias * Kp;
    return Turn;
}
