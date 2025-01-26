#pragma once

#include "app.h"

#if FTR_LED

typedef enum {

    LED_HANDLE_COUNT,
} LEDHandle;

void LED_setLedOn(LEDHandle led);
void LED_setLedOff(LEDHandle led);
void LED_setLedFreq(LEDHandle led, uint16_t hz);

void LED_task(void *);

#endif

