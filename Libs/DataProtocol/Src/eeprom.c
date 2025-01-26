
#include "eeprom.h"

#if FTR_DATASENDER
# include "st25r.h"
# include <cmsis_os.h>
#endif

volatile Eeprom eeprom;
#define EEPROM_ADDR( element ) (&element - &eeprom)

#if FTR_DATASENDER
extern osMessageQueueId_t nfcCommandQueueHandle;

# define NUM_NFC_COMMANDS 10
ST25R_command cmd[2];
static uint8_t flip = 0;

#define ACTIVE_CMD      cmd[flip]
#define FLIP_ACTIVE()   flip ^= 1;
#endif

bool Eeprom_writeData(uint16_t addr, eepromWord data)
{
#if FTR_DATASENDER
    ST25R_setCommand_writeSingleBlock(&ACTIVE_CMD, addr, data);
    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status == osOK)
    {
        FLIP_ACTIVE();
        return true;
    }
#endif
    return false;
}

bool Eeprom_readByte(uint16_t addr, uint32_t *readLoc)
{
#if FTR_DATASENDER
    ST25R_setCommand_readSingleBlock(&ACTIVE_CMD, addr, readLoc);
    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status == osOK)
    {
        FLIP_ACTIVE();
        return true;
    }
#endif
    return false;
}

bool Eeprom_readBytes(uint16_t addr, uint8_t len, uint32_t *readLoc)
{
#if FTR_DATASENDER
    ST25R_setCommand_readMultipleBlock(&ACTIVE_CMD, addr, len, readLoc);
    osStatus_t status = osMessageQueuePut(nfcCommandQueueHandle, &ACTIVE_CMD, 0, 10);

    if (status == osOK)
    {
        FLIP_ACTIVE();
        return true;
    }
#endif
    return false;
}

