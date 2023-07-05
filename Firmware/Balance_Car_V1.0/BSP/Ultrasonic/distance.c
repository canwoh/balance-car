#include"distance.h"

//mm_distance
float dist = 0;
//static uint32_t time_record[4];
uint32_t time_record[4];
/*
  * @brief  Echo Callback
  * @param  GPIO_Pin ：Echo_Pin
  * @retval 无
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
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
