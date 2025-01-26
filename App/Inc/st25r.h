#pragma once

#include "app.h"

#if FTR_NFCREADER
#include "m24lr.h"

#include <stdint.h>
#include <stdbool.h>

void ST25R_task(void *);
bool ST25R_connected(void);

// Commands
#define ST25R_MAX_PAYLOAD_SIZE  32U

typedef struct {
    NFCCommand cmd;
    bool completed;

    union
    {
        struct
        {
            uint8_t addr;

            uint8_t *rxLoc;
        } readSingleCmd;

        struct
        {
            uint8_t addr;
            uint32_t data;
        } writeSingleCmd;

        struct
        {
            uint8_t addr;
            uint8_t len;

            uint8_t *rxLoc;
        } readMultipleCmd;

        struct
        {
            uint8_t addr;
            uint8_t len;

            uint8_t *data;
        } writeMultipleCmd;
    };
} ST25R_command;
#endif