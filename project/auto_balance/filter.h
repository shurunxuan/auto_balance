/*
 * filter.h
 *
 *  Created on: 2018Äê3ÔÂ19ÈÕ
 *      Author: Nseven
 */

#ifndef FILTER_H_
#define FILTER_H_
float Yijiehubulvbo(float angle_m, float gyro_m, float dt);
float Erjiehubulvbo(float angle_m, float gyro_m, float dt);
float Kalman_Filter(float angle_m, float gyro_m, float dt);

#endif /* FILTER_H_ */
