
#include "button.h"

#if FTR_BUTTON

#include "cmsis_os.h"
#include "led.h"
#include "gui.h"

// Button Queue, when a button that is not currently in the queue is pressed, add it to the queue
extern osMessageQueueId_t buttonEventQueueHandle;

#define BUTTON_HOLD_TIME    2000    // ms

Button buttons[BUTTON_COUNT] = {
#if BOARD(CONTROLLER, 1)
    [BUTTON_A] = {
        .port = USER_BUTTON1_GPIO_Port,
        .pin = USER_BUTTON1_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },
    [BUTTON_B] = {
        .port = USER_BUTTON2_GPIO_Port,
        .pin = USER_BUTTON2_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },

    [BUTTON_UP] = {
        .port = USER_BUTTON6_GPIO_Port,
        .pin = USER_BUTTON6_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },
    [BUTTON_DOWN] = {
        .port = USER_BUTTON3_GPIO_Port,
        .pin = USER_BUTTON3_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },
    [BUTTON_LEFT] = {
        .port = USER_BUTTON5_GPIO_Port,
        .pin = USER_BUTTON5_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },
    [BUTTON_RIGHT] = {
        .port = USER_BUTTON4_GPIO_Port,
        .pin = USER_BUTTON4_Pin,

        .pressed = BUTTON_NOT_PRESSED,
        .held = false,
    },
#endif
};

// TODO: All button callbacks should be done via message queue (osMessageQueuePut -> From ISR)

static void inline Button_press(ButtonHandle handle)
{
    GUI_buttonCallback(handle, PRESS_SINGLE);
}

static void inline Button_release(ButtonHandle handle)
{

}

static void inline Button_hold(ButtonHandle handle)
{
    GUI_buttonCallback(handle, PRESS_LONG);
}

void Button_task(void *args)
{
    (void) args;

    for (;;)
    {
        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            if (buttons[i].pressed != BUTTON_NOT_PRESSED)
            {
                if (HAL_GetTick() - buttons[i].pressed > BUTTON_HOLD_TIME && !buttons[i].held)
                {
                    buttons[i].held = true;
                    Button_hold(i);
                }
            }
        }

        Watchdog_tickle(THREAD_BUTTON);
        osDelay(THREAD_BUTTON_PERIOD);
    }
}

void Button_EXTIRisingCallback(uint16_t GPIO_Pin)
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        if (buttons[i].pin == GPIO_Pin)
        {
            buttons[i].held = false;
            buttons[i].pressed = HAL_GetTick();
            // Button_press(i);
        }
    }
}

void Button_EXTIFallingCallback(uint16_t GPIO_Pin)
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        if (buttons[i].pin == GPIO_Pin)
        {
            buttons[i].pressed = BUTTON_NOT_PRESSED;
            // Button_release(i);
        }
    }
}


#endif
