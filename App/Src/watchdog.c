#include "watchdog.h"

#include "board.h"

#include <cmsis_os.h>
#include <stdbool.h>

#include "led.h"

#define WATCHDOG_DISABLED 0U
#define WATCHDOG_ALLOWED_SLOWDOWN   (0.9f)

static bool initialized = false;

static uint32_t reqTickles[THREAD_COUNT];
static uint32_t tickles[THREAD_COUNT];

static void Watchdog_clear(void)
{
    for (Thread thread = 0; thread < THREAD_COUNT; thread++)
    {
        if (reqTickles[thread] != WATCHDOG_DISABLED)
        {
            tickles[thread] = 0;
        }
    }
}

static void Watchdog_kill(volatile Thread thread)
{
    __disable_irq();
    volatile Thread killThread = thread;

    while(1)
    {
        for(uint32_t x = 0; x < 64e6/50; x++);
        LED_enableSolid(LED_WATCHDOG);
        for(uint32_t x = 0; x < 64e6/50; x++);
        LED_disable(LED_WATCHDOG);
    }
}

void Watchdog_register(Thread thread, uint32_t period)
{
    if (thread >= THREAD_COUNT)
        return; // Can't tickle non-existent task
    reqTickles[thread] = THREAD_WATCHDOG_PERIOD / period;
    tickles[thread] = 0;
}

void Watchdog_tickle(Thread thread)
{
    if (thread >= THREAD_COUNT)
        return; // Can't tickle non-existent task
    if (reqTickles[thread] == WATCHDOG_DISABLED)
        return; // Can't tickle disabled task

    tickles[thread]++;
}

// Highest priority task - important we dont spend too much time in here
void Watchdog_task(void *args)
{
    (void) args;

    // Start up time
    LED_enableSolid(LED_WATCHDOG);
    osDelay(4000);
    LED_enableHz(LED_WATCHDOG, 2);

    for (;;)
    {
        osDelay(THREAD_WATCHDOG_PERIOD);

        for (Thread thread = 0; thread < THREAD_COUNT; thread++)
        {
            if (reqTickles[thread] != WATCHDOG_DISABLED && tickles[thread] == 0)
            {
                Watchdog_kill(thread);
            }
        }

        Watchdog_clear();
    }
}
