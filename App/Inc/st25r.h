#pragma once

#include "app.h"

#if FTR_NFCREADER

#include <stdint.h>

// Commands
#define ST25R_CMD_SYS_INFO_REQ                       { 0x02, 0x2B }          /* NFC-V Get System Information command*/
#define ST25R_CMD_READ_SINGLE_BLOCK(addr)            { 0x02, 0x20, (addr)}   /* NFC-V Read single block command*/
#define ST25R_CMD_WRITE_SINGLE_BLOCK(addr, data)     { 0x02, 0x21, (addr), (((data) & 0xFF000000) >> 24), (((data) & 0x00FF0000) >> 16), (((data) & 0x0000FF00) >> 8), (((data) & 0x000000FF) >> 0)}   /* NFC-V Read single block command*/

typedef struct {
    uint8_t *payload;
    uint8_t payloadSize;
} ST25R_command;

void ST25R_main(void);
void ST25R_task(void *);

#endif
