#include"distance.h"

//mm_distance
float dist = 0;
static uint32_t time_record[4];

extern osThreadId_t ultrasonicHandle;

//freertos task for measuring distance
void Measure_Distance(void *args)
{
  HAL_GPIO_WritePin(Echo_GPIO_Port,Echo_Pin,GPIO_PIN_RESET);
  for(;;)
  {
    // safe_print("measure distance\r\n");
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET);
    mDelay_us(10);
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET);
    // uint32_t remain_mem = uxTaskGetStackHighWaterMark(ultrasonicHandle);
    // safe_print("%d\r\n",remain_mem);
    osDelay(100);
  }
    
}


/*
  * @brief  Echo Callback
  * @param  GPIO_Pin ：Echo_Pin
  * @retval 无
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
  if(GPIO_Pin == Echo_Pin)
  {
    //pull up trigger
    if(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==GPIO_PIN_SET)
    {
        time_record[1] = get_sysTick_ms();
        time_record[3] = get_sysTick_us_part();

    }
    //pull down trigger
    else if(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==GPIO_PIN_RESET)
    {
      //us 
        uint32_t total_time;
        // total_time = get_sysTick_us() - old_time;
        // dist = (float)total_time*0.17;
        time_record[0] = get_sysTick_ms();
        time_record[2] = get_sysTick_us_part();
        total_time = (time_record[0] - time_record[1])*1000 + time_record[3] - time_record[2]; 
        dist = (float)total_time * 0.17;
        // safe_print("total time us:%ld\r\n",total_time);
        // safe_print("new_us:%ld\r\n",time_record[0]);
        // safe_print("old_us:%ld\r\n",time_record[1]);
        // safe_print("new_us_part:%ld\r\n",time_record[2]);
        // safe_print("old_us_part:%ld\r\n",time_record[3]);
        // safe_print("dist = %f \r\n",dist);

    }
  }
}
