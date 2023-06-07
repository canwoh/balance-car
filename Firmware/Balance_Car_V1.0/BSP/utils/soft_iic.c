#include"soft_iic.h"


void delay_xus(__IO uint32_t nTime)
{
    int old_val, new_val, val;

    if (nTime > 900)
    {
        for (old_val = 0; old_val < nTime / 900; old_val++)
        {
            delay_xus(900);
        }
        nTime = nTime % 900;
    }

    old_val = SysTick->VAL;
    new_val = old_val - CPU_FREQUENCY_MHZ * nTime;
    if (new_val >= 0)
    {
        do
        {
            val = SysTick->VAL;
        } while ((val < old_val) && (val >= new_val));
    } else
    {
        new_val += CPU_FREQUENCY_MHZ * 1000;
        do
        {
            val = SysTick->VAL;
        } while ((val <= old_val) || (val > new_val));

    }
}

//---------------------Open-Drain-Mode--------------------------------
void SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = IIC_PIN_SDA;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_PORT_SDA, &GPIO_InitStruct);
}
void SCL_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = IIC_PIN_SCL;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_PORT_SCL, &GPIO_InitStruct);
}

void Soft_I2C_Init(void)
{
    SDA_Output();
    SCL_Output();
    SDA_Dout_High();
    SCL_Dout_High();
    Delay_us(2);
}

void Soft_I2C_Start(void)
{
    SDA_Dout_High();
    SCL_Dout_High();
    Delay_us(4);
    SDA_Dout_Low();
    Delay_us(4);
    SCL_Dout_Low();
    Delay_us(4);
}
void Soft_I2C_Stop(void)
{
    SDA_Dout_Low();
    SCL_Dout_Low();
    Delay_us(4);
    SCL_Dout_High();
    Delay_us(4);
    SDA_Dout_High();
    Delay_us(4);
}
void Soft_I2C_Send_Byte(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++){
        SDA_Dout_Write(data & (0x80>>i));
        Delay_us(4);
        SCL_Dout_High();
        Delay_us(4);
        SCL_Dout_Low();
        Delay_us(4);
    }  
}
uint8_t Soft_I2C_Read_Byte(void)
{
    uint8_t i;
    uint8_t recv = 0x00;
    SDA_Dout_High();
    Delay_us(4);
    for(i = 0; i < 8; i++){
        SCL_Dout_High();
        Delay_us(4);
        if(SDA_Data_In()){
            recv |= (0x80>>i);
        } 
        Delay_us(4);
        SCL_Dout_Low();
        Delay_us(4);
    }
    return recv;
}

void Soft_I2C_Ack(void)
{
    SDA_Dout_Low();
    Delay_us(2);
    SCL_Dout_High();
    Delay_us(2);
    SCL_Dout_Low();
    Delay_us(2); 
}
void Soft_I2C_NAck(void)
{
    SDA_Dout_High();
    Delay_us(2);
    SCL_Dout_High();
    Delay_us(2);
    SCL_Dout_Low();
    Delay_us(2); 
}

uint8_t Soft_I2C_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_Dout_High();
    Delay_us(2);
    SCL_Dout_High();
    Delay_us(2);
    while(SDA_Data_In())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            Soft_I2C_Stop();
            return 1;
        }
    }
    SCL_Dout_Low();
    return 0;
}

//addr = Slave address + W/R bit
void Soft_I2C_Master_Transmit(uint8_t addr,
                              uint8_t *pBuffer, 
                              uint8_t len)
{
    Soft_I2C_Start();
    Soft_I2C_Send_Byte(addr);
    Soft_I2C_Wait_Ack();
    for(int i = 0; i < len; i++)
    {
        Soft_I2C_Send_Byte(pBuffer[i]);
        Soft_I2C_Wait_Ack();
    }
    Soft_I2C_Stop();   
}



//---------------------Pull-Push-Mode-Help---------------------------------
// Configure the flow direction of the I2C bus

// void SDA_Output(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.Pin = IIC_PIN_SDA;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(IIC_PORT_SDA, &GPIO_InitStruct);
// }


// void SDA_Input(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.Pin = IIC_PIN_SDA;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(IIC_PORT_SDA, &GPIO_InitStruct);
// }

// void SCL_Output(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.Pin = IIC_PIN_SCL;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(IIC_PORT_SCL, &GPIO_InitStruct);
// }


// void SCL_Input(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.Pin = IIC_PIN_SCL;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(IIC_PORT_SCL, &GPIO_InitStruct);
// }


//----------------------------------------------------------------
