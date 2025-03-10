#pragma once

#include "app.h"

#if !FTR_NFCREADER
#define ST25R_connected()   true
#endif

#if FTR_NFCREADER
#include "m24lr.h"

#include <stdint.h>
#include <stdbool.h>

void ST25R_task(void *);
bool ST25R_connected(void);
void ST25R_EXTICallback(uint16_t GPIO_Pin);

#if defined(ST25R3916B)
int32_t ST25R_SequentialSend(const uint16_t DevAddr, const uint8_t * const pData, const uint16_t Length, const uint8_t last, const uint8_t txOnly);
int32_t ST25R_SequentialReceive(const uint16_t DevAddr, uint8_t * const pData, uint16_t Length);
#endif

// Commands
#define ST25R_MAX_PAYLOAD_SIZE  32U

typedef struct {
    NFCCommand cmd;

    union
    {
        struct
        {

        } sysInfoCmd;

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