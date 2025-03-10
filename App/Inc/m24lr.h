#pragma once

#include "app.h"

typedef enum
{
    NFC_COMMAND_INVALID = 0U,
    NFC_COMMAND_INVENTORY,
    NFC_COMMAND_READ_SINGLE_BLOCK,
    NFC_COMMAND_WRITE_SINGLE_BLOCK,
    NFC_COMMAND_READ_MULTIPLE_BLOCK,
    NFC_COMMAND_FAST_READ_SINGLE_BLOCK,
    NFC_COMMAND_FAST_READ_MULTIPLE_BLOCK,
    NFC_COMMAND_GET_SYS_INFO,
} NFCCommand;

#if FTR_NFCTAG
typedef enum
{
  M24LR_OK      = 0,
  M24LR_ERROR   = 1,
  M24LR_BUSY    = 2,
  M24LR_TIMEOUT = 3
} M24LR_StatusTypeDef;

M24LR_StatusTypeDef M24LR_i2c_IsDeviceReady( const uint32_t Trials );
M24LR_StatusTypeDef M24LR_i2c_ReadData( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
M24LR_StatusTypeDef M24LR_i2c_WriteData( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
#endif