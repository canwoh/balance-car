#include"SSD1306.h"


void SSD1306::OLED_Init()
{
    //上电延迟
    Delay_us(1000);
    Soft_I2C_Init();
    OLED_WriteCommand(0xAE);	//关闭显示

	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
    

}

void SSD1306::OLED_WriteCommand(uint8_t cmd)
{
    Soft_I2C_Start();
    Soft_I2C_Send_Byte(device_addr);
    Soft_I2C_Wait_Ack();
    Soft_I2C_Send_Byte(0x00);
    Soft_I2C_Wait_Ack();
    Soft_I2C_Send_Byte(cmd);
    Soft_I2C_Wait_Ack();
    Soft_I2C_Stop();
}

void SSD1306::OLED_Clear()
{
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/*
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
*/
void SSD1306::OLED_SetCursor(uint8_t x, uint8_t y)
{
	OLED_WriteCommand(0xB0 | y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((x & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (x & 0x0F));			//设置X位置低4位
}


/*
  * @brief  OLED显示字符
  * @param  y 以左上角为原点，向下方向的坐标，范围：0~3
  * @param  x 以左上角为原点，向右方向的坐标，范围：0~15
  * @param  ch Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
*/
void SSD1306::OLED_ShowChar(uint8_t x,uint8_t y,char ch)
{
    uint8_t i;
	OLED_SetCursor(y * 2, x * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[ch - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor(y * 2 + 1, x * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[ch - ' '][i + 8]);		//显示下半部分内容
	}
}

void SSD1306::OLED_WriteData(uint8_t data)
{
	Soft_I2C_Start();
	Soft_I2C_Send_Byte(device_addr);
    Soft_I2C_Wait_Ack();
	Soft_I2C_Send_Byte(0x40);
	Soft_I2C_Wait_Ack();
	Soft_I2C_Send_Byte(data);
	Soft_I2C_Wait_Ack();
	Soft_I2C_Stop();
}


/*
  * @brief  OLED显示字符
  * @param  y 以左上角为原点，向下方向的坐标，范围：0~3
  * @param  x 以左上角为原点，向右方向的坐标，范围：0~15
  * @param  str string 要显示的一个字符串，字符范围：ASCII可见字符
  * @param  len > 16 字符串会被截取
  * @retval 无
*/
void SSD1306::OLED_ShowString(uint8_t x, uint8_t y,char *str)
{
	uint8_t i;
	for(i = 0;str[i]!='\0'; i++)
	{
		OLED_ShowChar(x + i,y,str[i]);
	}
	
}

/*
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方 y>=0
*/
uint32_t SSD1306::OLED_Pow(uint32_t x, uint32_t y)
{
	if(y < 0)return -1;
	uint32_t res = 1;
	while(y--)
	{
		res *= x;
	}
	return res;
}

/*
  * @brief  OLED显示字符
  * @param  y 以左上角为原点，向下方向的坐标，范围：0~3
  * @param  x 以左上角为原点，向右方向的坐标，范围：0~15
  * @param  num count < 16
  * @retval 无
*/
void SSD1306::OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num)
{
	char str[15];
	sprintf(str,"%ld",num);
	OLED_ShowString(x,y,str);

}

/*
  * @brief  OLED显示字符
  * @param  y 以左上角为原点，向下方向的坐标，范围：0~3
  * @param  x 以左上角为原点，向右方向的坐标，范围：0~15
  * @param  num count < 16
  * @retval 无
*/
void SSD1306::OLED_ShowSignedNum(uint8_t x,uint8_t y,uint32_t num)
{
	char str[15];
	if(num >= 0)
	{
		sprintf(str,"+ %ld",num);
	}else {
		sprintf(str,"- %ld",num);
	}
	OLED_ShowString(x,y,str);

}