#pragma once

#include <stdint.h>
#include <assert.h>

typedef enum
{
    THREAD_WATCHDOG,
    THREAD_LED,
    THREAD_GUI,
    THREAD_ST25R,
    THREAD_ST25R_IRQ,
    THREAD_SENDER,
    THREAD_BUTTON,

    THREAD_COUNT
} Thread;


#define THREAD_WATCHDOG_PERIOD  1000

#define THREAD_LED_PERIOD       20
static_assert(THREAD_LED_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");

#define THREAD_GUI_PERIOD       150
static_assert(THREAD_GUI_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");

#define THREAD_ST25R_PERIOD     20
static_assert(THREAD_ST25R_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");

#define THREAD_ST25R_IRQ_PERIOD 10
static_assert(THREAD_ST25R_IRQ_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");

#define THREAD_SENDER_PERIOD    25
static_assert(THREAD_SENDER_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");

#define THREAD_BUTTON_PERIOD    100
static_assert(THREAD_BUTTON_PERIOD < THREAD_WATCHDOG_PERIOD, "Thread period MUST be smaller than watchdog period");


void Watchdog_register(Thread thread, uint32_t period);
void Watchdog_tickle(Thread thread);

void Watchdog_task(void *);
