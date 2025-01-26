
#include "led.h"

#if FTR_LED

#include "cmsis_os.h"

#define LED_TASK_FREQ 100 // [Hz]

#define HZ_TO_TICK(hz)  ((uint32_t) (1000 / (hz)))

void LED_setLedOn(LEDHandle led)
{
    // TODO
}

void LED_setLedOff(LEDHandle led)
{
    // TODO
}

void LED_setLedFreq(LEDHandle led, uint16_t hz)
{
    // TODO
}

void LED_setLedColor(LEDHandle led)
{
    // TODO (Later) how to set multi color LEDs where applicable
}

void LED_task(void *args)
{
    (void) args;

    for (;;)
    {
        // TODO

        osDelay(HZ_TO_TICK(LED_TASK_FREQ));
    }
}

#endif
