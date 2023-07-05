#ifndef __CPP_INV_MPU_H__
#define __CPP_INV_MPU_H__
#include"stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t MPU_Init(void);
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); 
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data);				
uint8_t MPU_Read_Byte(uint8_t reg);

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_LPF(uint16_t lpf);
uint8_t MPU_Set_Rate(uint16_t rate);
uint8_t MPU_Set_Fifo(uint8_t sens);

uint16_t MPU_Get_Temperature(void);
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az);

#ifdef __cplusplus
}
#endif

#endif