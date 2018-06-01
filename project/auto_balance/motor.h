/*
 * motor.h
 *
 *  Created on: 2018/01/11
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This header file includes the definitions
 *  used to drive the motors.
 *         The TB6612 can drive two motors at the
 *  same time, and it uses PWM to control the speed
 *  of the motors.
 *         The functions in this file can control
 *  the motors' speed, direction and on/off. There're
 *  also functions for getting the speed measured by
 *  the encoders.
 *
 */

#ifndef ___MOTOR_H_
#define ___MOTOR_H_


/* Application Defines */
#define TIMER_PERIOD 1000

/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void setup_pwm(void);
void set_pwm_1_duty(uint_fast16_t duty);
void set_pwm_2_duty(uint_fast16_t duty);
void set_pwm_duty(uint_fast16_t duty_1, uint_fast16_t duty_2);

void start_motor(void);
void stop_motor(void);
void set_motor_run(bool run);

void set_motor_1_forward(void);
void set_motor_1_backward(void);
void set_motor_1_direction(bool direction);

void set_motor_2_forward(void);
void set_motor_2_backward(void);
void set_motor_2_direction(bool direction);

int get_motor_1_speed(void);
int get_motor_2_speed(void);
extern int8_t motor_1_direction_late;
extern int8_t motor_2_direction_late;

#endif /* MOTOR_H_ */
