#ifndef __I2C_DEV_H__
#define __I2C_DEV_H__

#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif
#define IIC_WR	0		/* wirte control bit */
#define IIC_RD	1		/* read bit */

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t _ucByte);
uint8_t IIC_Read_Byte(uint8_t ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_CheckDevice(uint8_t _Address);
void IIC_GPIO_Init(void);
#ifdef __cplusplus
}
#endif

#endif
