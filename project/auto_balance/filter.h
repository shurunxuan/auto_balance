/*
 * filter.h
 *
 *  Created on: 2018��3��19��
 *      Author: Nseven
 */

#ifndef FILTER_H_
#define FILTER_H_
float Yijiehubulvbo(float angle_m, float gyro_m, float dt);
float Erjiehubulvbo(float angle_m, float gyro_m, float dt);
float Kalman_Filter(float angle_m, float gyro_m, float dt);

#endif /* FILTER_H_ */
