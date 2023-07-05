#ifndef __COMMON_INC_H__
#define __COMMON_INC_H__

#define CONFIG_FW_VERSION 1.0


/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus     
extern "C" {
#endif
    #include "FreeRTOS.h"
    #include "task.h"
    #include "main.h"
    #include "cmsis_os.h"
    #include "distance.h"
    void Main(void *argument);
    void IMU_Get_Data(void *argument);
    void Measure_Distance(void *argument);
#ifdef __cplusplus
}
//extern "C" void Main(void);
#endif 
/*---------------------------- C++ Scope ---------------------------*/

#endif