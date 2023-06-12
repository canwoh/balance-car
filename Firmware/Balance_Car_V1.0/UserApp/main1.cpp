#include"common_inc.h"
#include"SSD1306.h"


SSD1306 screen(OLED_ADDRESS);
extern osThreadId_t defaultTaskHandle;
/*------------------------------------------C++ scope------------------------------------*/

void Main()
{
    //screen.OLED_Init();
    //screen.OLED_ShowNum(0,0,1);
    for(;;)
    {
        safe_printf("hello world\r\n");
        uint32_t remain_mem = uxTaskGetStackHighWaterMark(defaultTaskHandle);
        safe_printf("%d\r\n",remain_mem);
        osDelay(1000);
        
    }
}

/*------------------------------------------C scope------------------------------------*/
#if defined(__cplusplus)
extern "C"{
#endif



#if defined(__cplusplus)
}
#endif