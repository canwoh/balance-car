#ifndef __SOFT_IIC_H__
#define __SOFT_IIC_H__

#ifdef __cplusplus  
extern "C"{   
#endif
// 本次配置为一主多从，采用开漏输出模式，若想使用推挽模式请自行修改。
#include "stm32f1xx_hal.h"
#include "gpio.h"

#define CPU_FREQUENCY_MHZ   72 //0

#define IIC_PIN_SDA         GPIO_PIN_8
#define IIC_PORT_SDA        GPIOA
#define IIC_PIN_SCL         GPIO_PIN_9
#define IIC_PORT_SCL        GPIOA


#define SDA_Dout_Low()              HAL_GPIO_WritePin(IIC_PORT_SDA, IIC_PIN_SDA,GPIO_PIN_RESET)
#define SDA_Dout_High()             HAL_GPIO_WritePin(IIC_PORT_SDA, IIC_PIN_SDA,GPIO_PIN_SET)
#define SDA_Data_In()               HAL_GPIO_ReadPin(IIC_PORT_SDA, IIC_PIN_SDA)
#define SCL_Dout_Low()              HAL_GPIO_WritePin(IIC_PORT_SCL, IIC_PIN_SCL,GPIO_PIN_RESET)
#define SCL_Dout_High()             HAL_GPIO_WritePin(IIC_PORT_SCL, IIC_PIN_SCL,GPIO_PIN_SET)
#define SCL_Data_In()               HAL_GPIO_ReadPin(IIC_PORT_SCL, IIC_PIN_SCL)
#define SDA_Dout_Write(STATES)      HAL_GPIO_WritePin(IIC_PORT_SDA,IIC_PIN_SDA,((STATES)?GPIO_PIN_SET:GPIO_PIN_RESET))

void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
void Soft_I2C_Send_Byte(uint8_t data);
uint8_t Soft_I2C_Read_Byte(void);

void Soft_I2C_Nack(void);
void Soft_I2C_Ack(void);
uint8_t Soft_I2C_Wait_Ack(void);

void delay_xus(__IO uint32_t nTime);

void Soft_I2C_Master_Transmit(uint8_t addr,uint8_t *pBuffer, uint8_t len);

#define Delay_us(nTime)     delay_xus((nTime))


#ifdef __cplusplus  
}
#endif
#endif /* __SOFT_IIC_H__ */