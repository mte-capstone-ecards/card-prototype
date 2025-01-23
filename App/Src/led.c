
#include "led.h"
#include "cmsis_os.h"

#define LED_TASK_FREQ 100 // [Hz]

#define HZ_TO_TICK(hz)  ((uint32_t) (1000 / (hz)))

void LED_setLedOn(LEDhandle led)
{
    // TODO
}

void LED_setLedOff(LEDhandle led)
{
    // TODO
}

void LED_setLedFreq(LEDhandle led, uint16_t hz)
{
    // TODO
}

void LED_setLedColor(LEDhandle led)
{
    // TODO (Later) how to set multi color LEDs where applicable
}

void LED_task(void *args)
{
    (void) args;

    for (;;)
    {
        // TODO

        osDelay(HZ_TO_TICKS(LED_TASK_FREQ));
    }
}

