#pragma once

#include "app.h"

#if FTR_DATASENDER

typedef struct
{
    uint32_t *data;
    uint8_t  startBit;
    uint8_t  numWords;
} SenderDataSpec;

void Sender_task(void *args);

#endif
