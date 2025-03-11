#pragma once

#include "app.h"

#if FTR_BUTTON

#include <stdbool.h>

typedef enum
{

    BUTTON_A,
    BUTTON_B,

    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,

    BUTTON_COUNT,
} ButtonHandle;

typedef enum
{
    PRESS_SINGLE,
    PRESS_LONG,
    PRESS_DOUBLE,
} PressType;
#define BUTTON_NOT_PRESSED  0x0

typedef struct
{
    ButtonHandle buttonHandle;

    PressType type;
} Button_event;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

    uint32_t pressed;
    bool held;
} Button;

extern Button buttons[BUTTON_COUNT];

#define BUTTON(b, t) (button == CONCAT(BUTTON_, b) && type == CONCAT(PRESS_, t))

void Button_task(void *args);

void Button_EXTIRisingCallback(uint16_t GPIO_Pin);
void Button_EXTIFallingCallback(uint16_t GPIO_Pin);

#endif
