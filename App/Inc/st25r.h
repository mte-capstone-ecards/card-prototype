#pragma once

#include "app.h"

#if FTR_NFCREADER

#include <stdint.h>
#include <stdbool.h>

void ST25R_task(void *);
bool ST25R_connected(void);

// Commands
#define ST25R_MAX_PAYLOAD_SIZE  32U

typedef struct {
    uint8_t payload[ST25R_MAX_PAYLOAD_SIZE];
    uint8_t payloadSize;

    uint8_t *rxBuf;
    uint8_t rxOffset;
    uint8_t rxLen;
} ST25R_command;

void ST25R_setCommand_getSysInfo(ST25R_command *cmd);
void ST25R_setCommand_readSingleBlock(ST25R_command *cmd, uint8_t addr, uint32_t *readLoc);
void ST25R_setCommand_writeSingleBlock(ST25R_command *cmd, uint8_t addr, uint32_t data);
void ST25R_setCommand_readMultipleBlock(ST25R_command *cmd, uint8_t addr, uint8_t len, uint32_t *readLoc);

#endif
