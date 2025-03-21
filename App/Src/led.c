
#include "led.h"

#if FTR_LED

#include "cmsis_os.h"
#include <gpio.h>
#include <stdbool.h>

#define HZ_TO_TICK(hz)  ((uint32_t) (1000 / (hz)))
#define HZ_TO_FREQ(hz)  ((1000 / hz) / THREAD_LED_PERIOD)

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;

    uint16_t freq;
    uint16_t clk;
    bool enabled;
} LED;

LED leds[LED_HANDLE_COUNT] = {
#if BOARD(CONTROLLER, 1)

    [LED_DISPLAY_R] = { // R10
        .port   = LED_DISPLAY_B_GPIO_Port,
        .pin    = LED_DISPLAY_B_Pin,

        .clk        = 0,
        .enabled    = false,
    },
    [LED_DISPLAY_G] = { // R11
        .port   = LED_DISPLAY_R_GPIO_Port,
        .pin    = LED_DISPLAY_R_Pin,

        .clk        = 0,
        .enabled    = false,
    },
    [LED_DISPLAY_B] = { // TEMP
        .port   = LED_DEBUG_B_GPIO_Port,
        .pin    = LED_DEBUG_B_Pin,

        .clk        = 0,
        .enabled    = false,
    },
#endif
#if BOARD(CONTROLLER, 0)
    [LED_NFC_WORKING] = {
        .port   = USER_LED_GPIO_Port,
        .pin    = USER_LED_Pin,

        .clk        = 0,
        .enabled    = false,
    }
#endif
};

void LED_enableSolid(LEDHandle handle)
{
    leds[handle].enabled = false;
    leds[handle].freq = 0;
    leds[handle].clk = 0;
    leds[handle].enabled = true;

    HAL_GPIO_WritePin(leds[handle].port, leds[handle].pin, GPIO_PIN_SET);
}

void LED_enableHz(LEDHandle handle, uint16_t hz)
{
    uint16_t newFreq = HZ_TO_FREQ(hz);
    if (leds[handle].freq != newFreq)
    {
        leds[handle].enabled = false;
        leds[handle].freq = newFreq;
        leds[handle].clk = 0;
        leds[handle].enabled = true;
    }
}

void LED_disable(LEDHandle handle)
{
    leds[handle].enabled = false;
    leds[handle].freq = 0;

    HAL_GPIO_WritePin(leds[handle].port, leds[handle].pin, GPIO_PIN_RESET);
}

void LED_task(void *args)
{
    (void) args;

    LED_enableHz(LED_WATCHDOG, 2);

    for (;;)
    {
        for (LEDHandle handle = LED_HANDLE_INITIAL; handle < LED_HANDLE_COUNT; handle++)
        {
            if (leds[handle].enabled)
            {
                if (leds[handle].freq == 0)
                {
                    HAL_GPIO_WritePin(leds[handle].port, leds[handle].pin, GPIO_PIN_SET);
                }
                else if (leds[handle].clk == leds[handle].freq)
                {
                    leds[handle].clk = 0;
                    HAL_GPIO_TogglePin(leds[handle].port, leds[handle].pin);
                }
                else
                {
                    leds[handle].clk++;
                }
            }
            else
            {
                HAL_GPIO_WritePin(leds[handle].port, leds[handle].pin, GPIO_PIN_RESET);
                leds[handle].clk = 0;
            }
        }

        Watchdog_tickle(THREAD_LED);
        osDelay(THREAD_LED_PERIOD);
    }
}

#endif
