/*
 * filter.c
 *
 *  Created on: 2018年3月19日
 *      Author: Nseven
 */

//一阶互补滤波

float K1 = 0.3f;//加速度计取值的权重
//float dt = 0.00093f;//滤波器采样时间
float angle1;

float Yijiehubulvbo(float angle_m,float gyro_m, float dt)
{
    angle1 = K1 * angle_m + (1 - K1) * (angle1 + gyro_m * dt);
    return angle1;
}

//二阶互补滤波
float K2=0.2;
float x1,x2,y1;
//float dt2=20*0.001;
float angle2;

float Erjiehubulvbo(float angle_m,float gyro_m, float dt)
{
    x1=(angle_m-angle2)*(1-K2)*(1-K2);
    y1=y1+x1*dt;
    x2=y1+2*(1-K2)*(angle_m-angle2)+gyro_m;
    angle2=angle2+x2*dt;
    return angle2;
}

//卡尔曼滤波
//float dt3=0.005;//kalman滤波器采样时间
float angle, angle_dot;//角度和角速度
float P[2][2] = {{ 1, 0 },
                 { 0, 1 }};
float Pdot[4] ={ 0,0,0,0};
float Q_angle=0.001, Q_gyro=0.005; //角度数据置信度,角速度数据置信度
float R_angle=0.5 ,C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;


//卡尔曼滤波
float Kalman_Filter(float angle_m, float gyro_m, float dt)
{
    angle+=(gyro_m-q_bias) * dt;
    angle_err = angle_m - angle;
    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1]=- P[1][1];
    Pdot[2]=- P[1][1];
    Pdot[3]=Q_gyro;
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    E = R_angle + C_0 * PCt_0;
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];
    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    angle += K_0 * angle_err; //最优角度
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m-q_bias;//最优角速度
    return angle;
}
