#include"common_inc.h"
//#include"SSD1306.h"
#include"mpu6050.h"

#define OLED_ADDRESS 0x78
//------------------------c header file scope----------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#include"inv_mpu.h"
#include"OLED_Driver.h"
#include<cstring>
#include"bluetooth.h"
#ifdef __cplusplus
}
#endif
//----------------------------------------------------------------

//SSD1306 screen(OLED_ADDRESS);
IMU_MPU6050 imu_6050;
//----------------------FreeRtos-----------
extern osMessageQueueId_t uart_rx_dma_queue_id;
extern osThreadId_t defaultTaskHandle;
extern osThreadId_t IMUHandle;
extern osThreadId_t ultrasonicHandle;
//----------------------debug code------
extern char debug_data[8];
uint8_t complete_record;
extern uint8_t test1;
// extern osThreadId_t defaultTaskHandle;
// extern osThreadId_t IMUHandle;
// extern osThreadId_t ultrasonicHandle;
// extern uint32_t time_record[4];
//---------------firmware code----------
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart3;
//-----------extern data----------------
extern float dist;
extern uint8_t uart_rx_dma_buffer[BUFFER_SIZE];
/*------------------------------------------C++ scope------------------------------------*/
void UART_Recv_DMA_Thread(void *args)
{
    // __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
    // __HAL_DMA_ENABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
    // __HAL_DMA_ENABLE_IT(&hdma_usart3_rx,DMA_IT_TC);
    // vTaskSuspendAll();
    // OLED_Clear();
    // OLED_ShowChar(4,7,'2');
    // xTaskResumeAll();
    HAL_UART_Receive_DMA(&huart3,uart_rx_dma_buffer,BUFFER_SIZE);
    complete_record = 2;
    void *d;
    for(;;)
    {   
        osMessageQueueGet(uart_rx_dma_queue_id, &d, NULL, osWaitForever);
        complete_record = 1;
        usart_tx_check();
        (void)d;
    }
}
//ultrasonic measuring distance  25Hz task
void Measure_Distance(void *args)
{
  HAL_GPIO_WritePin(Echo_GPIO_Port,Echo_Pin,GPIO_PIN_RESET);
  for(;;)
  {
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET);
    mDelay_us(15);   
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET);
    //uint32_t total_time = (time_record[0] - time_record[1])*1000 + time_record[3] - time_record[2]; 
    //uint32_t remain_mem = uxTaskGetStackHighWaterMark(ultrasonicHandle);
    //safe_printf("time : %ld\r\n",total_time);
    //safe_printf("dist : %.2f\r\n",dist);
    //safe_printf("remain_mem:%ld,dist:%f\r\n",remain_mem,dist);
    osDelay(40);
    
  }
    
}

//imu task handle function 200Hz task
void IMU_Get_Data(void *args)
{
    vTaskSuspendAll();
    while(mpu_dmp_init()){}

    //safe_printf("imu collect start\r\n");
    //printf("imu collect start\r\n");
    OLED_Init(); 
    xTaskResumeAll();
    for(;;)
    {
        vTaskSuspendAll();
        mpu_dmp_get_data(&imu_6050.imu_data_t.pitch,
                         &imu_6050.imu_data_t.roll,
                         &imu_6050.imu_data_t.yaw);
    
        xTaskResumeAll();
        //uint32_t mem = uxTaskGetStackHighWaterMark(IMUHandle);
        
        // safe_printf("ptich:%f,roll:%f,yaw:%f \r\n",
        //              imu_6050.imu_data_t.pitch,imu_6050.imu_data_t.roll,
        //              imu_6050.imu_data_t.yaw);
        //safe_printf("IMU water:%ld\r\n",mem);
        osDelay(5);
    }
}
//initialize and show data 5Hz task
void Main(void *args) 
{
    OLED_Init();
    char *msg = (char *)pvPortMalloc(16);
    for(;;)
    {
        vTaskSuspendAll();
        // OLED_ShowString(1,1,"mem:")
        memset(msg,0,16);
        sprintf(msg,"pitch:%.5f",imu_6050.imu_data_t.pitch);
        OLED_ShowString(1,1,msg);
        //printf("%s\r\n",msg);
        memset(msg,0,16);
        sprintf(msg,"roll:%.5f",imu_6050.imu_data_t.roll);
        OLED_ShowString(2,1,msg);
        //printf("%s\r\n",msg);
        memset(msg,0,16);
        sprintf(msg,"yaw:%.5f",imu_6050.imu_data_t.yaw);
        //OLED_ShowString(3,1,msg);
        //printf("%s\r\n",msg);
        // OLED_ShowSignedNum()
        OLED_ShowNum(3,1,complete_record,5);
        //uint8_t m_len = (uint8_t)(sizeof(debug_data)/sizeof(uint8_t));
        // for(uint8_t i = 1; i<2; i++)
        // {
            OLED_ShowChar(4,1,debug_data[0]);
            OLED_ShowChar(4,2,debug_data[1]);
        //}
        xTaskResumeAll();
        //uint32_t mem = uxTaskGetStackHighWaterMark(defaultTaskHandle);
        //safe_printf("default water:%ld\r\n",mem);
        osDelay(200);
        
    }
    //vPortFree(msg);
}

/*------------------------------------------C scope------------------------------------*/
#if defined(__cplusplus)
extern "C"{
#endif



#if defined(__cplusplus)
}
#endif