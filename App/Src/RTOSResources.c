#include "app.h"

#if OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "app.h"
#include "st25r.h"

// RTOS Mutexes

// RTOS Semaphores

// RTOS Timers

// RTOS Queues
extern osMessageQueueId_t nfcCommandQueueHandle;
osMessageQueueId_t nfcCommandQueueHandle;
const osMessageQueueAttr_t nfcCommandQueue_attributes = {
    .name = "nfcCommandQueue",
};

// RTOS Threads
osThreadId_t heartbeatTaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
  .name = "heartbeatTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t st25rTaskHandle;
const osThreadAttr_t st25rTask_attributes = {
  .name = "st25rTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

// RTOS Events



void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    //RTOS Mutexes

    // RTOS Semaphores

    // RTOS Timers

    // RTOS Queues
    nfcCommandQueueHandle = osMessageQueueNew(10, sizeof(ST25R_command), &nfcCommandQueue_attributes);

    // RTOS Threads
    heartbeatTaskHandle = osThreadNew(App_HeartbeatTask, NULL, &heartbeatTask_attributes);
    st25rTaskHandle     = osThreadNew(ST25R_task, NULL, &st25rTask_attributes);

    // RTOS Events

}
#endif
