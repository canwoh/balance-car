#include "cpp_inv_mpu.h"
#include "mpu6050.h"

extern IMU_MPU6050 imu_6050;

uint8_t MPU_Init(void)
{
    return imu_6050.MPU_Init();
}
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    return imu_6050.MPU_Write_Len(addr, reg, len, buf);
}
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    return imu_6050.MPU_Read_Len(addr, reg, len, buf);
}
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
    return imu_6050.MPU_Write_Byte(reg, data);
}
uint8_t MPU_Read_Byte(uint8_t reg)
{
    return imu_6050.MPU_Read_Byte(reg);
}

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return imu_6050.MPU_Set_Gyro_Fsr(fsr);
}
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return imu_6050.MPU_Set_Accel_Fsr(fsr);
}
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    return imu_6050.MPU_Set_LPF(lpf);
}
uint8_t MPU_Set_Rate(uint16_t rate)
{
    return imu_6050.MPU_Set_Rate(rate);
}
uint8_t MPU_Set_Fifo(uint8_t sens)
{
    return imu_6050.MPU_Set_Fifo(sens);
}

uint16_t MPU_Get_Temperature(void)
{
    return imu_6050.MPU_Get_Temperature();
}
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    return imu_6050.MPU_Get_Gyroscope(gx, gy, gz);
}
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    return imu_6050.MPU_Get_Accelerometer(ax, ay, az);
}
