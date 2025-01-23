#pragma once

#include "app.h"

#if FTR_LED

typedef enum {

    LED_HANDLE_COUNT,
} LEDhandle;

void LED_setLedOn(LEDhandle led);
void LED_setLedOff(LEDhandle led);
void LED_setLedFreq(LEDhandle led, uint16_t hz);

void LED_task(void *);

#endif

