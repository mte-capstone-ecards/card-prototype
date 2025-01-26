
#include "st25r.h"

#include <string.h>

#define ST25R_CMD_SYS_INFO_REQ                       { 0x02, 0x2B }          /* NFC-V Get System Information command*/
#define ST25R_CMD_READ_SINGLE_BLOCK(addr)            { 0x02, 0x20, (addr)}   /* NFC-V Read single block command*/
#define ST25R_CMD_WRITE_SINGLE_BLOCK(addr, data)     { 0x02, 0x21, (addr), (((data) & 0xFF000000) >> 24), (((data) & 0x00FF0000) >> 16), (((data) & 0x0000FF00) >> 8), (((data) & 0x000000FF) >> 0)}   /* NFC-V Read single block command*/

static void inline ST25R_clearCommand(ST25R_command *cmd)
{
    memset(cmd, 0U, sizeof(ST25R_command));
}

void ST25R_setCommand_getSysInfo(ST25R_command *cmd)
{
    ST25R_clearCommand(cmd);

    cmd->payload[0] = 0x02; // Flags
    cmd->payload[1] = 0x2B; // CMD

    cmd->payloadSize = 2;
}

void ST25R_setCommand_readSingleBlock(ST25R_command *cmd, uint8_t addr, uint32_t *readLoc)
{
    ST25R_clearCommand(cmd);

    cmd->payload[0] = 0x02; // Flags
    cmd->payload[1] = 0x20; // CMD
    cmd->payload[2] = addr; // Address

    cmd->payloadSize = 3;

    cmd->rxBuf = (uint8_t *) readLoc;
    cmd->rxOffset = 1U;
    cmd->rxLen = 4U;
}

void ST25R_setCommand_writeSingleBlock(ST25R_command *cmd, uint8_t addr, uint32_t data)
{
    ST25R_clearCommand(cmd);

    cmd->payload[0] = 0x02; // Flags
    cmd->payload[1] = 0x20; // CMD
    cmd->payload[2] = addr; // Address

    cmd->payload[3] = (data >> 24U) & 0xFF; // MSB Byte
    cmd->payload[4] = (data >> 16U) & 0xFF; // Byte 2
    cmd->payload[5] = (data >>  8U) & 0xFF; // Byte 3
    cmd->payload[6] = (data >>  0U) & 0xFF; // LSB Byte

    cmd->payloadSize = 7;
}

void ST25R_setCommand_readMultipleBlock(ST25R_command *cmd, uint8_t addr, uint8_t len, uint32_t *readLoc)
{
    ST25R_clearCommand(cmd);

    cmd->payload[0] = 0x02; // Flags
    cmd->payload[1] = 0x23; // CMD
    cmd->payload[2] = addr; // First address
    cmd->payload[3] = len - 1;  // Number of blocks

    cmd->payloadSize = 4;

    cmd->rxBuf = (uint8_t *) readLoc;
    cmd->rxOffset = 1U;
    cmd->rxLen = 4U * len;
}
