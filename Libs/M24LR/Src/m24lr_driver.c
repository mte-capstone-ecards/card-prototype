/**
  ******************************************************************************
  * @file    m24lr.c
  * @author  MMY Application Team
  * @version $Revision: 1577 $
  * @date    $Date: 2016-02-03 14:44:29 +0100 (Wed, 03 Feb 2016) $
  * @brief   This file provides set of driver functions to manage communication
  *          between BSP and M24LR chip.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "m24lr_driver.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */


/** @defgroup M24LR
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup M24LR_Private_Defines
  * @{
  */
#define M24LR_MAX_INSTANCE         1
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
M24LR_StatusTypeDef M24LR_i2c_Init( void );
M24LR_StatusTypeDef M24LR_i2c_ReadID( uint8_t * const pICRef );
M24LR_StatusTypeDef M24LR_i2c_IsDeviceReady( const uint32_t Trials );
M24LR_StatusTypeDef M24LR_i2c_ConfigureGPO( const uint16_t ITConf );
M24LR_StatusTypeDef M24LR_i2c_GetGPOStatus( uint16_t * const pGPOStatus );
M24LR_StatusTypeDef M24LR_i2c_ReadData( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
M24LR_StatusTypeDef M24LR_i2c_WriteData( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
M24LR_StatusTypeDef M24LR_i2c_ReadRegister( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
M24LR_StatusTypeDef M24LR_i2c_WriteRegister( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte );
M24LR_StatusTypeDef M24LR_i2c_ReadUID( M24LR_UID * const pUid );
M24LR_StatusTypeDef M24LR_i2c_ReadDSFID( uint8_t * const pDsfid );
M24LR_StatusTypeDef M24LR_i2c_ReadAFI( uint8_t * const pAfi );
M24LR_StatusTypeDef M24LR_i2c_ReadI2CLockSector( M24LR_Lock_Sectors * const pLock_sector );
M24LR_StatusTypeDef M24LR_i2c_I2CLockSector( const uint8_t Sector );
M24LR_StatusTypeDef M24LR_i2c_I2CUnlockSector( const uint8_t Sector );
M24LR_StatusTypeDef M24LR_i2c_PresentI2CPassword( const uint32_t PassWord );
M24LR_StatusTypeDef M24LR_i2c_WriteI2CPassword( const uint32_t PassWord );
M24LR_StatusTypeDef M24LR_i2c_ReadSSSx( const uint8_t SectorNb, M24LR_SECTOR_SEC * const pData );
M24LR_StatusTypeDef M24LR_i2c_WriteSSSx( const uint8_t SectorNb, const M24LR_SECTOR_SEC * const pData );
M24LR_StatusTypeDef M24LR_i2c_ReadMemSize( M24LR_Mem_Size * const pSizeInfo );
M24LR_StatusTypeDef M24LR_i2c_GetRF_WIP_BUSY( M24LR_GPO_STATUS * const pRf_Wip_Busy );
M24LR_StatusTypeDef M24LR_i2c_SetRFBUSY( void );
M24LR_StatusTypeDef M24LR_i2c_SetRFWIP( void );
M24LR_StatusTypeDef M24LR_i2c_Read_EH_mode( M24LR_EH_MODE_STATUS * const pEH_mode );
M24LR_StatusTypeDef M24LR_i2c_Enable_EH_mode( void );
M24LR_StatusTypeDef M24LR_i2c_Disable_EH_mode( void );
M24LR_StatusTypeDef M24LR_i2c_ReadEH_Cfg( M24LR_EH_CFG_VOUT * const pEH_Cfg );
M24LR_StatusTypeDef M24LR_i2c_WriteEH_Cfg( const M24LR_EH_CFG_VOUT EH_Cfg );
M24LR_StatusTypeDef M24LR_i2c_GetEH( M24LR_EH_STATUS * const pEH_Val );
M24LR_StatusTypeDef M24LR_i2c_SetEH( void );
M24LR_StatusTypeDef M24LR_i2c_ResetEH( void );
M24LR_StatusTypeDef M24LR_i2c_GetRFField( M24LR_FIELD_STATUS * const pRF_Field );
M24LR_StatusTypeDef M24LR_i2c_GetTProg( M24LR_T_PROG_STATUS * const pT_Prog );


/* Global variables ---------------------------------------------------------*/
/** @defgroup M24LR_Global_Variables
  * @{
  */
M24LR_DrvTypeDef M24lr_i2c_Drv =
{
  M24LR_i2c_Init,
  M24LR_i2c_ReadID,
  M24LR_i2c_IsDeviceReady,
  M24LR_i2c_ConfigureGPO,
  M24LR_i2c_GetGPOStatus,
  M24LR_i2c_ReadData,
  M24LR_i2c_WriteData,
  M24LR_i2c_ReadRegister,
  M24LR_i2c_WriteRegister,
  NULL
};

M24LR_ExtDrvTypeDef M24lr_i2c_ExtDrv =
{
  M24LR_i2c_ReadUID,
  M24LR_i2c_ReadDSFID,
  M24LR_i2c_ReadAFI,
  M24LR_i2c_ReadI2CLockSector,
  M24LR_i2c_I2CLockSector,
  M24LR_i2c_I2CUnlockSector,
  M24LR_i2c_PresentI2CPassword,
  M24LR_i2c_WriteI2CPassword,
  M24LR_i2c_ReadSSSx,
  M24LR_i2c_WriteSSSx,
  M24LR_i2c_ReadMemSize,
  M24LR_i2c_GetRF_WIP_BUSY,
  M24LR_i2c_SetRFBUSY,
  M24LR_i2c_SetRFWIP,
  M24LR_i2c_Read_EH_mode,
  M24LR_i2c_Enable_EH_mode,
  M24LR_i2c_Disable_EH_mode,
  M24LR_i2c_ReadEH_Cfg,
  M24LR_i2c_WriteEH_Cfg,
  M24LR_i2c_GetEH,
  M24LR_i2c_SetEH,
  M24LR_i2c_ResetEH,
  M24LR_i2c_GetRFField,
  M24LR_i2c_GetTProg
};

/* M24LR instances by address */
uint8_t M24lr[M24LR_MAX_INSTANCE] = {0};
/**
  * @}
  */

/* Public functions ---------------------------------------------------------*/
/** @defgroup M24LR_Public_Functions
  * @{
  */

/**
  * @brief  Set M24LR nfctag Initialization
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_Init( void )
{
  /* Configure the low level interface */
  return M24lr_IO_Init( );
}

/**
  * @brief  Read M24LR ID
  * @param  pICRef : pointer to store ID
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadID( uint8_t * const pICRef )
{
  /* Read ICRef on device */
  return M24LR_i2c_ReadRegister( pICRef, M24LR_ICREF_REG, 1 );
}

/**
  * @brief  Check M24LR availability
  * @param  Trials : number of max tentative tried
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_IsDeviceReady( const uint32_t Trials )
{
  /* Test i2c with M24LR */
  return M24lr_IO_IsDeviceReady( M24LR_ADDR_DATA_I2C, Trials );
}

/**
  * @brief  Configure M24LR GPO
  * @param  ITConf : 0x01 = RFBUSY, 0x02 = RFWIP
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ConfigureGPO( const uint16_t ITConf )
{
  M24LR_StatusTypeDef status = M24LR_ERROR;

  /* Configure GPO function on M24LR */
  if( (ITConf & M24LR_IT_BUSY_MASK) == M24LR_IT_BUSY_MASK )
  {
    status = M24LR_i2c_SetRFBUSY( );
  }
  else if( (ITConf & M24LR_IT_WIP_MASK) == M24LR_IT_WIP_MASK )
  {
    status = M24LR_i2c_SetRFWIP( );
  }
  return status;
}

/**
  * @brief  Get Configuration of M24LR GPO
  * @param  GPOStatus : 0x01 = RFBUSY, 0x02 = RFWIP
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_GetGPOStatus( uint16_t * const pGPOStatus )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract RF WIP/BUSY information */
  if( (reg_value & M24LR_CFG_WIPBUSY_MASK) == M24LR_CFG_WIPBUSY_MASK )
  {
    *pGPOStatus = M24LR_IT_WIP_MASK;
  }
  else
  {
    *pGPOStatus = M24LR_IT_BUSY_MASK;
  }

  return M24LR_OK;
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  NbByte : number of bytes to read
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadData( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  return M24lr_IO_MemRead( pData, M24LR_ADDR_DATA_I2C, TarAddr, NbByte );
}

/**
  * @brief  Write N data bytes starting from specified I2C Address
  * @brief  if I2C_Write_Lock bit = 0 or I2C_Password present => ack (modification OK)
  * @brief  if I2C_Write_Lock bit = 1  and no I2C_Password present => No ack (no modification)
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  NbByte : number of bytes to write
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteData( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  M24LR_StatusTypeDef status;
  uint8_t align_mem_offset;
  uint16_t bytes_to_write = NbByte;
  uint16_t mem_addr = TarAddr;
  const uint8_t *pdata_index = (const uint8_t *)pData;

  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  /* M24LR can write a maximum of 4 bytes in EEPROM per i2c communication */
  do
  {
    /* To write data in M24LR, data must be aligned on the same row in memory */
    /* align_mem_offset is used to copy only Bytes that are on the same row  in memory */
    if( bytes_to_write > M24LR_PAGEWRITE_NBBYTE )
    {
      /* DataSize higher than max page write, copy data by page */
      align_mem_offset = M24LR_PAGEWRITE_NBBYTE - (mem_addr % M24LR_PAGEWRITE_NBBYTE);
    }
    else
    {
      /* DataSize lower or equal to max page write, copy only last bytes */
      align_mem_offset = bytes_to_write;
    }
    /* Write align_mem_offset bytes in memory */
    status = M24lr_IO_MemWrite( pdata_index, M24LR_ADDR_DATA_I2C, mem_addr, align_mem_offset );
    /* update index, dest address, size for next write */
    pdata_index += align_mem_offset;
    mem_addr += align_mem_offset;
    bytes_to_write -= align_mem_offset;
    /* Poll until EEPROM is available */
    while( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK ) {};
  }
  while( ( bytes_to_write > 0 ) && ( status == M24LR_OK ) );

  return status;
}

/**
  * @brief  Read N register bytes starting from specified I2C address
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C memory address to read
  * @param  NbByte : number of bytes to read
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadRegister( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  return M24lr_IO_MemRead( pData, M24LR_ADDR_SYST_I2C, TarAddr, NbByte );
}

/**
  * @brief  Write N bytes to specific register
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C register address to write
  * @param  NbByte : number of bytes to write
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteRegister( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  M24LR_StatusTypeDef status;
  uint8_t align_mem_offset;
  uint16_t bytes_to_write = NbByte;
  uint16_t mem_addr = TarAddr;
  const uint8_t *pdata_index = (const uint8_t *)pData;

  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  /* M24LR can write a maximum of 4 bytes in EEPROM per i2c communication */
  do
  {
    /* To write data in M24LR, data must be aligned on the same row in memory */
    /* align_mem_offset is used to copy only Bytes that are on the same row  in memory */
    if( bytes_to_write > M24LR_PAGEWRITE_NBBYTE )
    {
      /* DataSize higher than max page write, copy data by page */
      align_mem_offset = M24LR_PAGEWRITE_NBBYTE - (mem_addr % M24LR_PAGEWRITE_NBBYTE);
    }
    else
    {
      /* DataSize lower or equal to max page write, copy only last bytes */
      align_mem_offset = bytes_to_write;
    }
    /* Write align_mem_offset bytes in register */
    status = M24lr_IO_MemWrite( pdata_index, M24LR_ADDR_SYST_I2C, mem_addr, align_mem_offset );
    /* update index, dest address, size for next write */
    pdata_index += align_mem_offset;
    mem_addr += align_mem_offset;
    bytes_to_write -= align_mem_offset;
    /* Poll until EEPROM is available */
    while( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK ) {};
  }
  while( ( bytes_to_write > 0 ) && ( status == M24LR_OK ) );

  return status;
}

/**
  * @brief  Read M24LR UID
  * @param  UID : M24LR_UID pointer of the UID to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadUID( M24LR_UID * const pUid )
{
  uint8_t areg_value[8];
  uint8_t i;
  M24LR_StatusTypeDef status;

  /* Read actual value of UID registers */
  status = M24LR_i2c_ReadRegister( areg_value, M24LR_UID_REG, 8 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Store information in 2 WORD */
  pUid->MSB_UID = 0;

  for( i = 0; i < 4; i++ )
  {
    pUid->MSB_UID = (pUid->MSB_UID << 8) | areg_value[7 - i];
  }

  pUid->LSB_UID = 0;

  for( i = 0; i < 4; i++ )
  {
    pUid->LSB_UID = (pUid->LSB_UID << 8) | areg_value[3 - i];
  }

  return M24LR_OK;
}

/**
  * @brief  Read DSFID
  * @param  pData : pointer of the DSFID to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadDSFID( uint8_t * const pDsfid )
{
  /* Read actual value of DSFID register */
  return M24LR_i2c_ReadRegister( pDsfid, M24LR_DSFID_REG, 1 );
}

/**
  * @brief  Read AFI
  * @param  pData : pointer of the AFI to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadAFI( uint8_t * const pAfi )
{
  /* Read actual value of AFI register */
  return M24LR_i2c_ReadRegister( pAfi, M24LR_AFI_REG, 1 );
}

/**
  * @brief  Read status of I2C Lock Sectors
  * @param  Lock_sector : M24LR_Lock_Sectors pointer of the I2c lock sector status to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadI2CLockSector( M24LR_Lock_Sectors * const pLock_sector )
{
  uint8_t areg_value[8];
  M24LR_StatusTypeDef status;

  /* Read actual value of I2c Write Lock registers */
  status = M24LR_i2c_ReadRegister( areg_value, M24LR_LOCK_REG, 8 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Dispatch information to corresponding struct member */
  pLock_sector->sectors_63_56 = areg_value[7];
  pLock_sector->sectors_55_48 = areg_value[6];
  pLock_sector->sectors_47_40 = areg_value[5];
  pLock_sector->sectors_39_32 = areg_value[4];
  pLock_sector->sectors_31_24 = areg_value[3];
  pLock_sector->sectors_23_16 = areg_value[2];
  pLock_sector->sectors_15_8 = areg_value[1];
  pLock_sector->sectors_7_0 = areg_value[0];

  return M24LR_OK;
}

/**
  * @brief  Lock I2C write on an EEPROM Sectors
  * @brief  Need a presentation of I2C Password to be effective
  * @param  Sector : EEPROM Sector number to lock (between 0 to 63)
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_I2CLockSector( const uint8_t Sector )
{
  M24LR_StatusTypeDef status;
  uint8_t reg_value = 0;
  uint16_t sector_write_lock_addr;

  /* Compute register address */
  sector_write_lock_addr = M24LR_LOCK_REG | (Sector >> 3);

  /* Read actual WriteLockStatus */
  status = M24LR_i2c_ReadRegister( &reg_value, sector_write_lock_addr, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Compute and update new WriteLockStatus */
  reg_value |= 1 << ( Sector % 8 );

  /* Write WriteLock register */
  return M24LR_i2c_WriteRegister( &reg_value, sector_write_lock_addr, 1 );
}

/**
  * @brief  UnLock I2C write on a EEPROM Sector
  * @brief  Need an presentation of I2C Password to be effective
  * @param  pSector : EEPROM Sector number to unlock (between 0 to 63)
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_I2CUnlockSector( const uint8_t Sector )
{
  M24LR_StatusTypeDef status;
  uint8_t reg_value = 0;
  uint16_t sector_write_lock_addr;

  /* Compute register address */
  sector_write_lock_addr = M24LR_LOCK_REG | (Sector >> 3);

  /* Read actual WriteLockStatus */
  status = M24LR_i2c_ReadRegister( &reg_value, sector_write_lock_addr, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Compute and update new WriteLockStatus */
  reg_value &= ~( 1 << ( Sector % 8 ) );

  /* Write WriteLock register */
  return M24LR_i2c_WriteRegister( &reg_value, sector_write_lock_addr, 1 );
}

/**
  * @brief  Present I2C password, authorize I2C write
  * @param  PassWord : Password value on 32bits
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_PresentI2CPassword( const uint32_t PassWord )
{
  uint8_t ai2c_message[9] = {0};
  uint8_t i;

  /* Build I2C Message with Password + Validation code 0x09 + Password */
  ai2c_message[4] = 0x09;
  i = 0;
  while( i < 4 )
  {
    ai2c_message[i] = ( PassWord >> (i * 8) ) & 0xFF;
    ai2c_message[i + 5] = ( PassWord >> (i * 8) ) & 0xFF;
    i++;
  };

  /* Present password to M24LR */
  return M24lr_IO_MemWrite( ai2c_message, M24LR_ADDR_SYST_I2C, M24LR_I2C_PWD_REG, 9 );
}

/**
  * @brief  Write new I2C password
  * @brief  Need to present good I2CPassword before using this function
  * @param  PassWord : new I2C PassWord value on 32bits
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteI2CPassword( const uint32_t PassWord )
{
  uint8_t ai2c_message[9] = {0};
  uint8_t i;

  /* Build I2C Message with Password + Validation code 0x07 + Password */
  ai2c_message[4] = 0x07;
  i = 0;
  while( i < 4 )
  {
    ai2c_message[i] = ( PassWord >> (i * 8) ) & 0xFF;
    ai2c_message[i + 5] = ( PassWord >> (i * 8) ) & 0xFF;
    i++;
  };

  /* Write Password to register */
  return M24LR_i2c_WriteRegister( ai2c_message, M24LR_I2C_PWD_REG, 9 );
}

/**
  * @brief  Read SectorSecurityStatus (defining RF access allowed)
  * @param  SectorNb : Sector number to get RF security status
  * @param  pData : M24LR_SECTOR_SEC pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadSSSx( const uint8_t SectorNb, M24LR_SECTOR_SEC * const pData )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;
  uint16_t sector_security_addr;

  /* Compute Sector Security register address */
  sector_security_addr = M24LR_SSS_REG | SectorNb;

  /* Read actual value of SectorSecurityStatus register */
  status = M24LR_i2c_ReadRegister( &reg_value, sector_security_addr, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract Sector Security Status configuration */
  pData->SectorLock = reg_value & M24LR_SSS_LOCK_MASK;
  pData->RW_Protection = (reg_value & M24LR_SSS_RW_MASK) >> 1;
  pData->PassCtrl = (reg_value & M24LR_SSS_PASSCTRL_MASK) >> 3;

  return M24LR_OK;
}

/**
  * @brief  Write SectorSecurityStatus (defining RF access allowed)
  * @brief  Need an presentation of I2C Password to be effective
  * @param  SectorNb : Sector number to set RF security
  * @param  pData : M24LR_SECTOR_SEC pointer of the data to write
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteSSSx( const uint8_t SectorNb, const M24LR_SECTOR_SEC * const pData )
{
  uint8_t reg_value;
  uint16_t sector_security_addr;

  /* Compute Sector Security register address */
  sector_security_addr = M24LR_SSS_REG | SectorNb;

  /* Update Sector Security Status */
  reg_value = (pData->PassCtrl << 3) & M24LR_SSS_PASSCTRL_MASK;
  reg_value |= ((pData->RW_Protection << 1) & M24LR_SSS_RW_MASK);
  reg_value |= (pData->SectorLock & M24LR_SSS_LOCK_MASK);

  /* Write SectorSecurityStatus register */
  return M24LR_i2c_WriteRegister( &reg_value, sector_security_addr, 1 );
}

/**
  * @brief  Read Memory Size info
  * @param  SizeInfo : M24LR_Mem_Size pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadMemSize( M24LR_Mem_Size * const pSizeInfo )
{
  uint8_t areg_value[3];
  M24LR_StatusTypeDef status;

  /* Read actual value of Mem_Size register */
  status = M24LR_i2c_ReadRegister( areg_value, M24LR_MEMSIZE_REG, 3 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract Mem information */
  pSizeInfo->BlockSize = areg_value[2];
  pSizeInfo->Mem_Size = areg_value[1];
  pSizeInfo->Mem_Size = (pSizeInfo->Mem_Size << 8) | areg_value[0];

  return M24LR_OK;
}

/**
  * @brief  Get GPO Configuration status
  * @param  Rf_Wip_Busy : M24LR_GPO_STATUS pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_GetRF_WIP_BUSY( M24LR_GPO_STATUS * const pRf_Wip_Busy )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract RF WIP/BUSY information */
  if( (reg_value & M24LR_CFG_WIPBUSY_MASK) == M24LR_CFG_WIPBUSY_MASK )
  {
    *pRf_Wip_Busy = M24LR_GPO_WIP;
  }
  else
  {
    *pRf_Wip_Busy = M24LR_GPO_BUSY;
  }

  return M24LR_OK;
}

/**
  * @brief  Configure GPO as RF Busy
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_SetRFBUSY( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24lr_IO_MemRead( &reg_value, M24LR_ADDR_SYST_I2C, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update register value for BUSY configuration */
  reg_value &= !M24LR_CFG_WIPBUSY_MASK;

  /* Write CFG register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CFG_REG, 1 );
}

/**
  * @brief  Configure GPO as RF WriteInProgress
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_SetRFWIP( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update register value for WIP configuration */
  reg_value |= M24LR_CFG_WIPBUSY_MASK;

  /* Write CFG register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CFG_REG, 1 );
}

/**
  * @brief  Get Energy harvesting mode status
  * @param  EH_mode : M24LR_EH_MODE_STATUS pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_Read_EH_mode( M24LR_EH_MODE_STATUS * const pEH_mode )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract EH_mode configuration */
  if( (reg_value & M24LR_CFG_EHMODE_MASK) == M24LR_CFG_EHMODE_MASK )
  {
    *pEH_mode = M24LR_EH_MODE_DISABLE;
  }
  else
  {
    *pEH_mode = M24LR_EH_MODE_ENABLE;
  }

  return M24LR_OK;
}

/**
  * @brief  Enable Energy harvesting mode
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_Enable_EH_mode( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update EH_mode */
  reg_value &= ~M24LR_CFG_EHMODE_MASK;

  /* Write CFG register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CFG_REG, 1 );
}

/**
  * @brief  Disable Energy harvesting mode
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_Disable_EH_mode( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update EH_mode */
  reg_value |= M24LR_CFG_EHMODE_MASK;

  /* Write CFG register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CFG_REG, 1 );
}

/**
  * @brief  Read Vout sink current configuration status for Energy Harvesting
  * @param  EH_Cfg : M24LR_EH_CFG_VOUT pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ReadEH_Cfg( M24LR_EH_CFG_VOUT * const pEH_Cfg )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract Vout configuration for EH information */
  reg_value &= (M24LR_CFG_EHCFG1_MASK | M24LR_CFG_EHCFG0_MASK);
  switch( reg_value )
  {
    case 0:
      *pEH_Cfg = M24LR_EH_Cfg_6MA;
      break;
    case 1:
      *pEH_Cfg = M24LR_EH_Cfg_3MA;
      break;
    case 2:
      *pEH_Cfg = M24LR_EH_Cfg_1MA;
      break;
    case 3:
      *pEH_Cfg = M24LR_EH_Cfg_300UA;
      break;

    default:
      *pEH_Cfg = M24LR_EH_Cfg_6MA;
  }

  return M24LR_OK;
}

/**
  * @brief  Write Vout sink current configuration status for Energy Harvesting
  * @param  EH_Cfg : M24LR_EH_CFG_VOUT value to configure Vout
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteEH_Cfg( const M24LR_EH_CFG_VOUT EH_Cfg )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CFG register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CFG_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update Vout configuration */
  reg_value &= ~(M24LR_CFG_EHCFG1_MASK | M24LR_CFG_EHCFG0_MASK);
  reg_value |= EH_Cfg;

  /* Write CFG register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CFG_REG, 1 );
}

/**
  * @brief  Get Energy Harvesting status
  * @param  EH_Val : M24LR_EH_STATUS pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_GetEH( M24LR_EH_STATUS * const pEH_Val )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CTRL register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CTRL_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Extract EH information */
  if( (reg_value & M24LR_CTRL_EHEN_MASK) == M24LR_CTRL_EHEN_MASK )
  {
    *pEH_Val = M24LR_EH_ENABLE;
  }
  else
  {
    *pEH_Val = M24LR_EH_DISABLE;
  }

  return M24LR_OK;
}

/**
  * @brief  Enable Energy Harvesting
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_SetEH( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CTRL register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CTRL_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update EH configuration */
  reg_value |= M24LR_CTRL_EHEN_MASK;

  /* Write CTRL Register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CTRL_REG, 1 );
}

/**
  * @brief  Disable Energy Harvesting
  * @param  None
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_ResetEH( void )
{
  uint8_t reg_value;
  M24LR_StatusTypeDef status;

  /* Read actual value of CTRL register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CTRL_REG, 1 );
  if( status != M24LR_OK )
  {
    return status;
  }

  /* Update EH configuration */
  reg_value &= ~M24LR_CTRL_EHEN_MASK;

  /* Write CTRL register */
  return M24LR_i2c_WriteRegister( &reg_value, M24LR_CTRL_REG, 1 );
}

/**
  * @brief  Check if RF Field is present in front of M24LR
  * @param  pRF_Field :  M24LR_FIELD_STATUS pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_GetRFField( M24LR_FIELD_STATUS * const pRF_Field )
{
  M24LR_StatusTypeDef status;
  uint8_t reg_value = 0;

  /* Read actual value of CTRL register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CTRL_REG, 1 );

  /* Extract RF Field information */
  if( status == M24LR_OK )
  {
    if( (reg_value & M24LR_CTRL_FIELD_MASK) == M24LR_CTRL_FIELD_MASK )
    {
      *pRF_Field = M24LR_FIELD_ON;
    }
    else
    {
      *pRF_Field = M24LR_FIELD_OFF;
    }

    return M24LR_OK;
  }

  return status;
}

/**
  * @brief  Check if Write Timing is good
  * @param  pT_Prog : M24LR_T_PROG_STATUS pointer of the data to store
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_GetTProg( M24LR_T_PROG_STATUS * const pT_Prog )
{
  M24LR_StatusTypeDef status;
  uint8_t reg_value = 0;

  /* Read actual value of CTRL register */
  status = M24LR_i2c_ReadRegister( &reg_value, M24LR_CTRL_REG, 1 );

  /* Extract T-Prog information */
  if( status == M24LR_OK )
  {
    if( (reg_value & M24LR_CTRL_TPROG_MASK) == M24LR_CTRL_TPROG_MASK )
    {
      *pT_Prog = M24LR_T_PROG_OK;
    }
    else
    {
      *pT_Prog = M24LR_T_PROG_NO;
    }

    return M24LR_OK;
  }
  else
    return status;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
