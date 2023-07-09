#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include<string.h>
#include "main.h"
#include "OLED_Driver.h"
#include "common_inc.h"
#include "stm32f1xx_hal.h"


#define BUFFER_SIZE 8
#define ARRAY_LEN(x)    (sizeof(x)/sizeof((x)[0]))


void usart_tx_check(void);


#endif