#pragma once

#include "app.h"

#if FTR_DATASENDER
#include "eeprom.h"

typedef struct
{
    SenderInstruction instr;

    union {
        struct {
            uint8_t suit;
            uint8_t num;
        } card;

        struct {
            uint8_t indices[4];
            char str[255];
            uint8_t len;
        } string;
    };
} SenderDataSpec;

void Sender_task(void *args);

#endif
