#pragma once

#include "app.h"

#if FTR_LED

typedef enum {
    LED_HANDLE_INITIAL  = 0,

    LED_DISPLAY_R       = LED_HANDLE_INITIAL,
    LED_DISPLAY_B,

    LED_DEBUG_B,
    LED_NFC_WORKING,
    LED_NFC_DONE,

    LED_HANDLE_COUNT,
} LEDHandle;

void LED_enableSolid(LEDHandle handle);
void LED_enableHz(LEDHandle handle, uint16_t hz);
void LED_disable(LEDHandle handle);

void LED_task(void *);

#endif

