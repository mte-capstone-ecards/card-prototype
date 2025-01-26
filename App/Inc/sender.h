#pragma once

#include "app.h"

#if FTR_DATASENDER

typedef struct
{
    uint8_t *data;
    uint8_t dataLen;
} SenderDataSpec;

void Sender_task(void *args);

#endif
