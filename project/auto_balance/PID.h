/*
 * PID.h
 *
 *  Created on: 2018Äê3ÔÂ27ÈÕ
 *      Author: Nseven
 */

#ifndef PID_H_
#define PID_H_

int balance(float Angle , float Gyro);
int velocity(int encoder_left, int encoder_right);
int turn(float gyro);



#endif /* PID_H_ */
