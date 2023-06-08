#include"common_inc.h"
#include"SSD1306.h"


SSD1306 screen(OLED_ADDRESS);

/*------------------------------------------C++ scope------------------------------------*/

void Main()
{
    
    HAL_Delay(1000);
    screen.OLED_Init();
    screen.OLED_ShowChar(0,0,'1');
    
}

/*------------------------------------------C scope------------------------------------*/
#if defined(__cplusplus)
extern "C"{
#endif



#if defined(__cplusplus)
}
#endif