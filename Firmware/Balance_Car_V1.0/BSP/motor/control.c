#include "control.h"



extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


Motor_PID_ARGS PID_ARGS = {
    .Balance_Kp = -240, //raw value:400 Processed value : -240
    .Balance_Ki = 0,//UNUSED
    .Balance_Kd = -0.68,  //raw value:1.1 Processed value : -0.67
    .Velocity_Kp = -135,
    .Velocity_Ki = -0.67,
    .Velocity_Kd = 0,//UNUSED
    .Turn_Kp = 0,//UNUSED
    .Turn_Ki = 0,//UNUSED
    .Turn_Kd = 0,//UNUSED
};

// 读取编码器值
//@param Motorx(1 or 2) : 1)：编码器1 TIM2  2)：编码器2 TIM4
int Read_Encoder(uint8_t Motorx)
{
    int Encoder_Value;
    switch (Motorx)
    {
    case 1:
        Encoder_Value = (short)TIM2->CNT;
        TIM2->CNT = 0;
        break;
    case 2:
        Encoder_Value = (short)TIM4->CNT;
        TIM4->CNT = 0;
        break;
    default:
        Encoder_Value = 0;
    }
    return Encoder_Value;
}

/***************************************
 *@name：直立PD控制
 *@param：角度、机械平衡角度（机械中值）、角速度
 *@return：直立控制PWM
 *
 */
int Vertical_Ring_PD(float pitch, float Gypo)
{
    float Bias;  // 角度误差
    int balance; // 直立环计算出来的电机控制pwm
    Bias = pitch - MECHANICAL_MEDIAN_ANGLE;
    balance = PID_ARGS.Balance_Kp * Bias + PID_ARGS.Balance_Kd * Gypo;
    return balance;
}


int Velocity_Ring_PI(int Encoder_Left, int Encoder_Right,float pitch)
{
    float Velocity, Encoder_Least;
    static float Encoder, Encoder_Integral;
    // static float Movement;
    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）
    Encoder_Least = (Encoder_Left + Encoder_Right) - 0;
    Encoder *= 0.8; // 一阶低通滤波
    Encoder += Encoder_Least * 0.2;
    Encoder_Integral += Encoder;
    // 积分限幅
    if (Encoder_Integral > 10000)
        Encoder_Integral = 10000;
    if (Encoder_Integral < -10000)
        Encoder_Integral = -10000;
    // 速度控制

    Velocity = PID_ARGS.Velocity_Kp * Encoder + PID_ARGS.Velocity_Ki * Encoder_Integral;
    if(pitch<-68||pitch>68)Encoder_Integral=0;
    return Velocity;
}

void Motor_Limit(int *PWM1, int *PWM2)
{
    int m_pwm1 = *PWM1;
    int m_pwm2 = *PWM2;
    if (m_pwm1 > 0)
    {
        *PWM1 = m_pwm1 > 3034 ? 3034 : m_pwm1;
    }
    else
    {
        *PWM1 = m_pwm1 < -3034 ? -3034 : m_pwm1;
    }
    if (m_pwm2 > 0)
    {
        *PWM2 = m_pwm2 > 3034 ? 3034 : m_pwm2;
    }
    else
    {
        *PWM2 = m_pwm2 < -3034 ? -3034 : m_pwm2;
    }
}


int ABS(int a)
{
	a = a > 0 ? a : (-a);
	return a;
}

uint8_t Turn_OFF(float pitch)
{
    if(pitch<-68||pitch>68)
    {
        return 1;
    }
    return 0;
}

void Set_PWM(int motor1,int motor2)
{
    if(motor1>0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,MOTOR_DEAD_ZONE+ABS(motor1)*1.17);  
    }else
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,MOTOR_DEAD_ZONE+ABS(motor1)*1.17);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
    }
    if(motor2>0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,MOTOR_DEAD_ZONE+ABS(motor1)*1.17);  
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,MOTOR_DEAD_ZONE+ABS(motor1)*1.17);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
    }

}

void Motor_Init(void)
{

    __HAL_TIM_ENABLE_IT(&htim3,TIM_IT_UPDATE);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_4);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
}