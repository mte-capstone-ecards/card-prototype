#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t shape;
    uint8_t num;
    uint32_t *data;
    uint32_t  startBit;
    uint32_t  numWords;
} SenderDataSpec;
