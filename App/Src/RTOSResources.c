#include "app.h"

#if OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "st25r.h"
#include "led.h"
#include "button.h"
#include "sender.h"

#include "gui.h"
#include "watchdog.h"

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
typedef struct
{
  osThreadId_t handle;
  void (*task)(void *);
  const char *name;
  uint32_t stack_size;
  osPriority_t priority;
  uint32_t period;
} ThreadConfig;

ThreadConfig threads[THREAD_COUNT] = {
#if FTR_WATCHDOG
  [THREAD_WATCHDOG] = {
    .task       = Watchdog_task,
    .name       = "watchdogTask",
    .stack_size = 128 * 2,
    .priority   = osPriorityHigh2,
    .period     = THREAD_WATCHDOG_PERIOD
  },
#endif
#if FTR_LED
  [THREAD_LED] = {
    .task       = LED_task,
    .name       = "ledTask",
    .stack_size = 128 * 2,
    .priority   = osPriorityLow,
    .period     = THREAD_LED_PERIOD,
  },
#endif
#if FTR_GUI
  [THREAD_GUI] = {
    .task       = GUI_task,
    .name       = "guiTask",
    .stack_size = 128 * 4,
    .priority   = osPriorityNormal,
    .period     = THREAD_GUI_PERIOD,
  },
#endif
#if FTR_NFCREADER
  [THREAD_ST25R] = {
    .task       = ST25R_task,
    .name       = "st25rTask",
    .stack_size = 128 * 8,
    .priority   = osPriorityHigh,
    .period     = THREAD_ST25R_PERIOD,
  },
#endif
#if defined(ST25R3916B)
  [THREAD_ST25R_IRQ] = {
    .task       = ST25R_irqTask,
    .name       = "st25rIrqTask",
    .stack_size = 128 * 2,
    .priority   = osPriorityHigh1,
    .period     = THREAD_ST25R_IRQ_PERIOD,
  },
#endif
#if FTR_DATASENDER
  [THREAD_SENDER] = {
    .task       = Sender_task,
    .name       = "senderTask",
    .stack_size = 128 * 8,
    .priority   = osPriorityNormal1,
    .period     = THREAD_SENDER_PERIOD,
  },
#endif
#if FTR_BUTTON
  [THREAD_BUTTON] = {
    .task       = Button_task,
    .name       = "buttonTask",
    .stack_size = 128 * 2,
    .priority   = osPriorityLow1,
    .period     = THREAD_BUTTON_PERIOD,
  },
#endif
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
#if FTR_NFCREADER
    nfcCommandQueueHandle   = osMessageQueueNew(20, sizeof(ST25R_command), &nfcCommandQueue_attributes);
#endif
#if FTR_DATASENDER
    dataSenderQueueHandle   = osMessageQueueNew(1, sizeof(SenderDataSpec), &dataSenderQueue_attributes);
#endif
#if FTR_BUTTON
    buttonEventQueueHandle  = osMessageQueueNew(10, sizeof(Button_event), &buttonEventQueue_attributes);
#endif

    // RTOS Threads
    for (Thread thread = 0; thread < THREAD_COUNT; thread++)
    {
      if (threads[thread].task != NULL)
      {
        const osThreadAttr_t attr = { // TODO: We should statically allocate each threads stack
          .name       = threads[thread].name,
          .stack_size = threads[thread].stack_size,
          .priority   = threads[thread].priority,
        };

        threads[thread].handle = osThreadNew(threads[thread].task, NULL, &attr);

        if (threads[thread].handle == NULL) // Failed to create a thread. Critical error
        {
          Error_Handler();
        }

        if (thread != THREAD_WATCHDOG)
        {
          Watchdog_register(thread, threads[thread].period);
        }
      }
    }

    // RTOS Events

}
#endif
