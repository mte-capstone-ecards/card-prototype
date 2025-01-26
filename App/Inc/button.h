#pragma once

#include "app.h"

#if FTR_BUTTON

#include <stdbool.h>

typedef enum
{

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

#endif
