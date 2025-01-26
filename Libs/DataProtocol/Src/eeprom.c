
#include "eeprom.h"

volatile Eeprom eeprom;
#define EEPROM_ADDR( element ) ((uint8_t) ((uint8_t *) &element - (uint8_t *) &eeprom))

#if FTR_DATASENDER
# include "st25r.h"
# include "sender.h"
# include <cmsis_os.h>
# include <string.h>

extern osMessageQueueId_t nfcCommandQueueHandle;
extern osSemaphoreId_t nfcCommandSemaphore;

# define NUM_NFC_COMMANDS 10
ST25R_command cmd[2];
static uint8_t flip = 0;

# define ACTIVE_CMD      cmd[flip]
# define FLIP_ACTIVE()   flip ^= 1;

static void inline clearCmd(ST25R_command *cmd)
{
    memset(cmd, 0U, sizeof(ST25R_command));
}

#endif

static bool Eeprom_writeBlock(uint16_t addr, eepromWord data)
{
#if FTR_DATASENDER
    clearCmd(&ACTIVE_CMD);
    ACTIVE_CMD.cmd = NFC_COMMAND_WRITE_SINGLE_BLOCK;
    ACTIVE_CMD.writeSingleCmd.addr = addr;
    ACTIVE_CMD.writeSingleCmd.data = data;

    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status != osOK)
        return false;

    FLIP_ACTIVE();
    return true;
#endif
    return false;
}

static bool Eeprom_readBlock(uint16_t addr, volatile uint32_t *readLoc)
{
#if FTR_DATASENDER
    clearCmd(&ACTIVE_CMD);
    ACTIVE_CMD.cmd = NFC_COMMAND_READ_SINGLE_BLOCK;
    ACTIVE_CMD.readSingleCmd.addr = addr;
    ACTIVE_CMD.readSingleCmd.rxLoc = (uint8_t *) readLoc;

    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status != osOK)
        return false;

    FLIP_ACTIVE();
    return true;
#endif
    return false;
}

static bool Eeprom_readBlocks(uint16_t addr, uint8_t len, volatile uint32_t *readLoc)
{
#if FTR_DATASENDER
    clearCmd(&ACTIVE_CMD);
    ACTIVE_CMD.cmd = NFC_COMMAND_READ_MULTIPLE_BLOCK;
    ACTIVE_CMD.readMultipleCmd.addr = addr;
    ACTIVE_CMD.readMultipleCmd.len = len - 1;
    ACTIVE_CMD.readMultipleCmd.rxLoc = (uint8_t *) readLoc;

    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status != osOK)
        return false;

    FLIP_ACTIVE();
    return true;
#endif
    return false;
}

bool Eeprom_waiting()
{
    return cmd[0].completed && cmd[1].completed;
}

bool Eeprom_readSector(uint8_t sector)
{
    return Eeprom_readBlocks(32U * sector, 32, eeprom.sectors[sector]);
}

bool Eeprom_writeNextSeqId()
{
    uint8_t block = EEPROM_ADDR(eeprom.header.senderCmdBytes.seqNum) / 4;
    uint8_t offset = 4 - (EEPROM_ADDR(eeprom.header.senderCmdBytes.seqNum) % 4);

    uint8_t existingData = *(((uint32_t *) &eeprom) + block) & ~(0xFF << (8U * offset));
    uint8_t nextSeqId = eeprom.header.readerCfg.seqNum + 1;

    return Eeprom_writeBlock(block, existingData | (nextSeqId << (8U * offset)));
}

bool Eeprom_writeData(uint8_t dataAddr, uint32_t data)
{
    return Eeprom_writeBlock(EEPROM_ADDR(eeprom.data.u32[data]) / 4, data);
}