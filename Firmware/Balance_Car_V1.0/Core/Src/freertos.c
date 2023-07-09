/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"common_inc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osSemaphoreId debug_print;
osMessageQueueId_t uart_rx_dma_queue_id;
osThreadId_t ultrasonicHandle;
osThreadId_t IMUHandle;
osThreadId_t bluetoothHandle;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;

uint8_t test1 = 0;

const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Ultrasonic_Task(void *argument);
void IMU_Get_Measure_Task(void *argument);
void Bluetooth_Task(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  osSemaphoreDef(debug_print);
  debug_print = osSemaphoreNew(1,1,osSemaphore(debug_print));

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  uart_rx_dma_queue_id = osMessageQueueNew(10, sizeof(void *), NULL);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  
  const osThreadAttr_t ultrasonicThreadAtt = {
      .name = "ultrasonic",
      .stack_size = 128*6 ,
      .priority = (osPriority_t)osPriorityNormal,
  };
  const osThreadAttr_t IMUThreadAtt = {
      .name = "imu",
      .stack_size = 128*5,
      .priority = (osPriority_t)osPriorityNormal,
  };
  const osThreadAttr_t bluetoothThreadAtt = {
      .name="bluetooth",
      .stack_size=128*10,
      .priority=(osPriority_t)osPriorityNormal,
  };
  
  //IMUHandle = osThreadNew(IMU_Get_Measure_Task,NULL,&IMUThreadAtt);
  bluetoothHandle = osThreadNew(Bluetooth_Task,NULL,&bluetoothThreadAtt);
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  ultrasonicHandle = osThreadNew(Ultrasonic_Task,NULL,&ultrasonicThreadAtt);
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  Main(argument);
  vTaskDelete(defaultTaskHandle);
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Ultrasonic_Task(void *argument)
{
  Measure_Distance(argument);
  vTaskDelete(ultrasonicHandle);
}

void IMU_Get_Measure_Task(void *argument)
{
   IMU_Get_Data(argument);
   vTaskDelete(IMUHandle);
}

void Bluetooth_Task(void *argument)
{
  test1 = 1;
  UART_Recv_DMA_Thread(argument);
  vTaskDelete(bluetoothHandle);
}
/* USER CODE END Application */

