#include "common_inc.h"
#include "mpu6050.h"

#define OLED_ADDRESS 0x78
//------------------------c header file scope----------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif
#include "inv_mpu.h"
#include "OLED_Driver.h"
#include <cstring>
#include "bluetooth.h"
#include"control.h"
#ifdef __cplusplus
}
#endif
//----------------------------------------------------------------

// SSD1306 screen(OLED_ADDRESS);
IMU_MPU6050 imu_6050;
//----------------------FreeRtos-----------
extern osMessageQueueId_t uart_rx_dma_queue_id;
extern osThreadId_t defaultTaskHandle;
extern osThreadId_t IMUHandle;
extern osThreadId_t ultrasonicHandle;
//----------------------debug code------
extern char debug_data[8];
uint8_t complete_record;

uint8_t motor_run = 0;
// extern osThreadId_t defaultTaskHandle;
// extern osThreadId_t IMUHandle;
// extern osThreadId_t ultrasonicHandle;
// extern uint32_t time_record[4];
//-----------------User data------------
extern float dist;
extern uint8_t uart_rx_dma_buffer[BUFFER_SIZE];

int balance_pwm,velocity_pwm;
int encoder_left, encoder_right;
int motor1_pwm,motor2_pwm;
//---------------firmware code----------
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim3;
/*------------------------------------------C++ scope------------------------------------*/
void Motion_Controller_Thread(void *args)
{
    Motor_Init();
    // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
    // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
    // __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,MOTOR_DEAD_ZONE);
    // __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,MOTOR_DEAD_ZONE);
    for(;;)
    {
        //读编码器
        encoder_left = Read_Encoder(1);
        encoder_right = -Read_Encoder(2); 
        //直立环
        balance_pwm = Vertical_Ring_PD(imu_6050.imu_data_t.pitch,
                                       imu_6050.imu_data_t.gyro_y);
        //速度环
        velocity_pwm = Velocity_Ring_PI(encoder_left, encoder_right,imu_6050.imu_data_t.pitch);
        motor1_pwm = balance_pwm+velocity_pwm;
        motor2_pwm = balance_pwm+velocity_pwm;
        Motor_Limit(&motor1_pwm,&motor2_pwm);
        if(Turn_OFF(imu_6050.imu_data_t.pitch)){
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);  
            __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);  
        }else {
            Set_PWM(motor1_pwm,motor2_pwm);
        }
        osDelay(10);
    }
}

void UART_Recv_DMA_Thread(void *args)
{
    
    HAL_UART_Receive_DMA(&huart3, uart_rx_dma_buffer, BUFFER_SIZE);
    complete_record = 2;
    void *d;
    for (;;)
    {
        osMessageQueueGet(uart_rx_dma_queue_id, &d, NULL, osWaitForever);
        complete_record = 1;
        usart_tx_check();
        (void)d;
    }
}
// ultrasonic measuring distance  25Hz task
void Measure_Distance(void *args)
{
    HAL_GPIO_WritePin(Echo_GPIO_Port, Echo_Pin, GPIO_PIN_RESET);
    for (;;)
    {
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
        mDelay_us(15);
        HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
        osDelay(40);
    }
}

// imu task handle function 200Hz task
void IMU_Get_Data(void *args)
{
    vTaskSuspendAll();
    while (mpu_dmp_init())
    {
    }

    OLED_Init();
    xTaskResumeAll();
    for (;;)
    {
        vTaskSuspendAll();
        mpu_dmp_get_data(&imu_6050.imu_data_t);

        xTaskResumeAll();
        osDelay(5);
    }
}
// initialize and show data 5Hz task
// void Main(void *args)
// {
//     OLED_Init();
//     Motor_Init();
//     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
//     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
//     __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,MOTOR_DEAD_ZONE);
//     __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,MOTOR_DEAD_ZONE);
//     //char *msg = (char *)pvPortMalloc(16);
//     for (;;)
//     {
//         vTaskSuspendAll();
//         // OLED_ShowString(1,1,"mem:")
//         // memset(msg, 0, 16);
//         // sprintf(msg, "pitch:%.5f", imu_6050.imu_data_t.pitch);
//         // OLED_ShowString(1, 1, msg);
//         // // printf("%s\r\n",msg);
//         // memset(msg, 0, 16);
//         // sprintf(msg, "roll:%.5f", imu_6050.imu_data_t.roll);
//         // OLED_ShowString(2, 1, msg);
//         // // printf("%s\r\n",msg);
//         // memset(msg, 0, 16);
//         // sprintf(msg, "yaw:%.5f", imu_6050.imu_data_t.yaw);
//         // OLED_ShowString(3,1,msg);


//         OLED_ShowNum(4,1,motor_run,4);
      
//         xTaskResumeAll();
//         osDelay(200);
//     }
//     // vPortFree(msg);
// }

