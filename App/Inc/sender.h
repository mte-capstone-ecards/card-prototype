#pragma once

#include "app.h"

#if FTR_DATASENDER

typedef struct
{
#if FORCE_HANABI
    uint8_t shape;
    uint8_t num;
#endif
    uint32_t *data;
    uint32_t  startBit;
    uint32_t  numWords;
} SenderDataSpec;

void Sender_task(void *args);

#endif
