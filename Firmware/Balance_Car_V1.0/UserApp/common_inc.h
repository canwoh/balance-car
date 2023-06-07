#ifndef __COMMON_INC_H__
#define __COMMON_INC_H__

#define CONFIG_FW_VERSION 1.0
#define OLED_ADDRESS 0x78

/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus     
extern "C" {
#endif

    #include "FreeRTOS.h"
    #include "task.h"
    #include "main.h"
    #include "cmsis_os.h"

    void Main(void);
#ifdef __cplusplus
}
//extern "C" void Main(void);
#endif 
/*---------------------------- C++ Scope ---------------------------*/

#endif