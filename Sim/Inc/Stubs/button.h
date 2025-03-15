#pragma once

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

#define CONCAT(A, B) A ## B
#define BUTTON(b, t) (button == CONCAT(BUTTON_, b) && type == CONCAT(PRESS_, t))
