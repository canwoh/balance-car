#ifndef __CONTORL_H__
#define __CONTORL_H__
#include "stm32f1xx_hal.h"

#define MOTOR_DEAD_ZONE 3300      // 电机死区
#define MECHANICAL_MEDIAN_ANGLE -2 // 机械中值pitch角

typedef struct Motor_PID_ARGS
{
    float Balance_Kp;
    float Balance_Ki;
    float Balance_Kd;

    float Velocity_Kp;
    float Velocity_Ki;
    float Velocity_Kd;

    float Turn_Kp;
    float Turn_Ki;
    float Turn_Kd;

} Motor_PID_ARGS;
extern Motor_PID_ARGS PID_ARGS;

void Motor_Init(void);
int Vertical_Ring_PD(float pitch, float Gypo);
int Velocity_Ring_PI(int Encoder_Left, int Encoder_Right,float pitch);
int Read_Encoder(uint8_t Motorx);
void Motor_Limit(int *PWM1, int *PWM2);
int ABS(int a);
uint8_t Turn_OFF(float pitch);
void Set_PWM(int motor1,int motor2);

#endif