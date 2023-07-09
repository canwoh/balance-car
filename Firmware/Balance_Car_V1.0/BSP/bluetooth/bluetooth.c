#include "bluetooth.h"
#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

//-----------------debug---------------
char debug_data[8];


//--------------------------------------

extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart3;
//extern osMessageQueueId_t uart_rx_dma_queue_id;

uint8_t uart_rx_dma_buffer[BUFFER_SIZE];
static volatile uint16_t uart_rx_index = 0;
static volatile uint8_t uart_rx_complete = 0;

void uart_process_data(const void *data, size_t len);

void usart_tx_check()
{
    static size_t old_pos;
    size_t pos;
    pos = ARRAY_LEN(uart_rx_dma_buffer) - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
    if (pos != old_pos)
    { /* Check change in received data */
        if (pos > old_pos)
        { /* Current position is over previous one */
            /*
             * Processing is done in "linear" mode.
             *
             * Application processing is fast with single data block,
             * length is simply calculated by subtracting pointers
             *
             * [   0   ]
             * [   1   ] <- old_pos |------------------------------------|
             * [   2   ]            |                                    |
             * [   3   ]            | Single block (len = pos - old_pos) |
             * [   4   ]            |                                    |
             * [   5   ]            |------------------------------------|
             * [   6   ] <- pos
             * [   7   ]
             * [ N - 1 ]
             */
            uart_process_data(&uart_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            /*
             * Processing is done in "overflow" mode..
             *
             * Application must process data twice,
             * since there are 2 linear memory blocks to handle
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | Second block (len = pos)        |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | First block (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */
            uart_process_data(&uart_rx_dma_buffer[old_pos], ARRAY_LEN(uart_rx_dma_buffer) - old_pos);
            if (pos > 0)
            {
                uart_process_data(&uart_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos; /* Save current position as old for next transfers */
        HAL_UART_Receive_DMA(&huart3,uart_rx_dma_buffer,BUFFER_SIZE);
    }
}

void uart_process_data(const void *data, size_t len)
{
    memcpy(debug_data,data,len);
}
//-----------------------Callback Functions& Interrupt Handler Functions---------------
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//    void *d = (void*)1;
//    if((__HAL_UART_GET_IT_SOURCE(huart,UART_IT_IDLE))&&(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)))
//    {
//         __HAL_UART_CLEAR_IDLEFLAG(huart);
//         complete_record = 1;
//         osMessageQueuePut(uart_rx_dma_queue_id,&d,0,0);
//    }
// }

// void UART_DMA_IRQHandler(DMA_HandleTypeDef* huart)
// {
//     void *d=(void*)1;
//     if((__HAL_DMA_GET_IT_SOURCE(huart,DMA_IT_HT))&&(__HAL_DMA_GET_FLAG(huart,DMA_FLAG_HT3)))
//     {

//         __HAL_DMA_CLEAR_FLAG(huart,DMA_FLAG_HT3);
//         complete_record = 1;
//         osMessageQueuePut(huart,&d,0,0);
//     }
//     if((__HAL_DMA_GET_IT_SOURCE(huart,DMA_IT_TC))&&(__HAL_DMA_GET_FLAG(huart,DMA_FLAG_TC3)))
//     {
//         __HAL_DMA_CLEAR_FLAG(huart,DMA_FLAG_TC3);
//         complete_record = 1;
//         osMessageQueuePut(huart,&d,0,0);
//     }
// }