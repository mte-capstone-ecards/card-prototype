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

typedef struct
{
    ButtonHandle buttonHandle;

    PressType type;
} Button_event;

#define BUTTON(b, t) (button == CONCAT(BUTTON_, b) && type == CONCAT(PRESS_, t))

void Button_task(void *args);

#endif
