
#include "eeprom.h"

volatile Eeprom eeprom;

#if FTR_DATASENDER
# include "st25r.h"
# include "sender.h"
# include <cmsis_os.h>
# include <string.h>

extern osMessageQueueId_t nfcCommandQueueHandle;

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

#if FTR_DATARECEIVER
#include "m24lr_driver.h"
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
#elif FTR_DATARECEIVER
    return M24lr_i2c_Drv.WriteData((uint8_t *) &data, addr * 4, 4) == M24LR_OK;
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
#elif FTR_DATARECEIVER
    return M24lr_i2c_Drv.ReadData((uint8_t *) readLoc, addr * 4, 4) == M24LR_OK;
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
#elif FTR_DATARECEIVER
    return M24lr_i2c_Drv.ReadData((uint8_t *) readLoc, addr * 4, 4 * len) == M24LR_OK;
#endif
    return false;
}

bool Eeprom_waiting()
{
#if FTR_DATASENDER
    return osMessageQueueGetCount(nfcCommandQueueHandle) != 0U;
#elif FTR_DATARECEIVER
    return false;
#endif
}

bool Eeprom_readSector(uint8_t sector)
{
    return Eeprom_readBlocks(32U * sector, 32, eeprom.sectors[sector]);
}

bool Eeprom_writeNextSeqId()
{
#if FTR_DATASENDER
    eeprom.senderHeader.seqNum = eeprom.receiverHeader.seqNum + 1;
    return Eeprom_writeBlock(0, *((uint32_t *) &eeprom.senderHeader));
#elif FTR_DATARECEIVER
    eeprom.receiverHeader.seqNum = eeprom.senderHeader.seqNum + 1;
    return Eeprom_writeBlock(1, *((uint32_t *) &eeprom.receiverHeader));
#endif
}

bool Eeprom_readSenderHeader()
{
    return Eeprom_readBlock(0, (uint32_t *) &eeprom.senderHeader);
}

bool Eeprom_readReceiverHeader()
{
    return Eeprom_readBlock(1, (uint32_t *) &eeprom.receiverHeader);
}

bool Eeprom_writeData(uint8_t dataAddr, uint32_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        if (!Eeprom_writeBlock(2 + dataAddr + i, data[i]))
            return false;
    }
    return true;
}