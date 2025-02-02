#include "app.h"

#if OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "st25r.h"
#include "led.h"
#include "button.h"
#include "sender.h"

// RTOS Mutexes

// RTOS Semaphores

// RTOS Timers

// RTOS Queues
#if FTR_DATASENDER
extern osMessageQueueId_t nfcCommandQueueHandle;
osMessageQueueId_t nfcCommandQueueHandle;
const osMessageQueueAttr_t nfcCommandQueue_attributes = {
    .name = "nfcCommandQueue",
};

extern osMessageQueueId_t dataSenderQueueHandle;
osMessageQueueId_t dataSenderQueueHandle;
const osMessageQueueAttr_t dataSenderQueue_attributes = {
    .name = "dataSenderQueue",
};
#endif

#if FTR_BUTTON
extern osMessageQueueId_t buttonEventQueueHandle;
osMessageQueueId_t buttonEventQueueHandle;
const osMessageQueueAttr_t buttonEventQueue_attributes = {
    .name = "buttonEventQueue",
};
#endif

// RTOS Threads
osThreadId_t heartbeatTaskHandle;
const osThreadAttr_t heartbeatTask_attributes = {
  .name = "heartbeatTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

#if FTR_NFCREADER
osThreadId_t st25rTaskHandle;
const osThreadAttr_t st25rTask_attributes = {
  .name = "st25rTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
#endif

#if FTR_DATASENDER
osThreadId_t senderTaskHandle;
const osThreadAttr_t senderTask_attributes = {
  .name = "senderTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
#endif

#if FTR_LED
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 256 * 2,
  .priority = (osPriority_t) osPriorityLow,
};
#endif

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
#if FTR_DATASENDER
    nfcCommandQueueHandle   = osMessageQueueNew(1, sizeof(ST25R_command), &nfcCommandQueue_attributes);
    dataSenderQueueHandle   = osMessageQueueNew(1, sizeof(SenderDataSpec), &dataSenderQueue_attributes);
#endif
#if FTR_BUTTON
    buttonEventQueueHandle  = osMessageQueueNew(10, sizeof(Button_event), &buttonEventQueue_attributes);
#endif

    // RTOS Threads
    heartbeatTaskHandle = osThreadNew(Controller_hearbeatTask, NULL, &heartbeatTask_attributes);
#if FTR_NFCREADER
    st25rTaskHandle     = osThreadNew(ST25R_task, NULL, &st25rTask_attributes);
#endif
#if FTR_DATASENDER
    senderTaskHandle       = osThreadNew(Sender_task, NULL, &senderTask_attributes);
#endif
#if FTR_LED
    ledTaskHandle       = osThreadNew(LED_task, NULL, &ledTask_attributes);
#endif

    // RTOS Events

}
#endif
