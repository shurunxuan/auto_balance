/*
 * PID.h
 *
 *  Created on: 2018��3��27��
 *      Author: Nseven
 */

#ifndef PID_H_
#define PID_H_

int balance(float Angle , float Gyro);
int velocity(int encoder_left, int encoder_right);
int turn(float gyro);



#endif /* PID_H_ */
