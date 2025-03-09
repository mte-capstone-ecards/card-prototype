
#include "led.h"

#if FTR_LED

#include "cmsis_os.h"
#include <gpio.h>
#include <stdbool.h>

#define LED_TASK_FREQ 100 // [Hz]

#define HZ_TO_TICK(hz)  ((uint32_t) (1000 / (hz)))
#define HZ_TO_FREQ(hz)  (LED_TASK_FREQ / (hz))

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;

    uint16_t freq;
    uint16_t clk;
    bool enabled;
} LED;

LED leds[LED_HANDLE_COUNT] = {
#if BOARD(CONTROLLER, 1)
    [LED_DISPLAY_R] = { // R11
        .port   = LED_DISPLAY_R_GPIO_Port,
        .pin    = LED_DISPLAY_R_Pin,

        .enabled = false,
    },
    [LED_DISPLAY_B] = { // R10
        .port   = LED_DISPLAY_B_GPIO_Port,
        .pin    = LED_DISPLAY_B_Pin,

        .enabled = false,
    },

    [LED_NFC_WORKING] = { // R8
        .port   = LED_DEBUG_R_GPIO_Port,
        .pin    = LED_DEBUG_R_Pin,

        .enabled = false,
    },
    [LED_NFC_DONE] = { // R9
        .port   = LED_DEBUG_G_GPIO_Port,
        .pin    = LED_DEBUG_G_Pin,

        .enabled = false,
    },
    [LED_DEBUG_B] = { // R7
        .port   = LED_DEBUG_B_GPIO_Port,
        .pin    = LED_DEBUG_B_Pin,

        .enabled = false,
    },
#endif
};

void LED_enableSolid(LEDHandle handle)
{
    leds[handle].enabled = false;
    leds[handle].freq = 0;
    leds[handle].clk = 0;
    leds[handle].enabled = true;
}

void LED_enableHz(LEDHandle handle, uint16_t hz)
{
    leds[handle].enabled = false;
    leds[handle].freq = HZ_TO_FREQ(hz);
    leds[handle].clk = 0;
    leds[handle].enabled = true;
}

void LED_disable(LEDHandle handle)
{
    leds[handle].enabled = false;
    leds[handle].freq = 0;
}

void LED_task(void *args)
{
    (void) args;

    for (LEDHandle handle = LED_HANDLE_INITIAL; handle < LED_HANDLE_COUNT; handle++)
    {
        LED_disable(handle);
    }

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

        osDelay(HZ_TO_TICK(LED_TASK_FREQ));
    }
}

#endif
