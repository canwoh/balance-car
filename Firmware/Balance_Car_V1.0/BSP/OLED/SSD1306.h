#ifndef __SSD1306__
#define __SSD1306__

#include"soft_iic.h"
#include"stm32f1xx_hal.h"
#include"OLED_font.h"
#include"string.h"
#include"stdlib.h"
#include"stdio.h"

class SSD1306 
{
    private:
    uint8_t device_addr;
    public:
    explicit SSD1306(uint8_t s_addr):device_addr(s_addr) {}

    public:
    void OLED_Init();
    void OLED_Clear();
    void OLED_WriteCommand(uint8_t cmd);
    void OLED_ShowChar(uint8_t x,uint8_t y,char ch);
    void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num);
    void OLED_ShowString(uint8_t x, uint8_t y,char* str);
    void OLED_ShowSignedNum(uint8_t x, uint8_t y, uint32_t num);
    private:
    void OLED_SetCursor(uint8_t x, uint8_t y);
    void OLED_WriteData(uint8_t data);
    uint32_t OLED_Pow(uint32_t x, uint32_t y);

};

#endif