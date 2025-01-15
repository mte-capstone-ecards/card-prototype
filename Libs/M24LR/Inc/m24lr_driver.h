/**
  ******************************************************************************
  * @file    m24lr.h
  * @author  MMY Application Team
  * @version $Revision: 1577 $
  * @date    $Date: 2016-02-03 14:44:29 +0100 (Wed, 03 Feb 2016) $
  * @brief   This file provides set of driver functions to manage communication
  * @brief   between MCU and M24LR chip
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _M24LR_H
#define _M24LR_H

#ifdef _cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup M24LR
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup M24LR_Exported_Types
  * @{
  */
/**
 * @brief  M24LR_ status enumerator definition
 */
typedef enum
{
  M24LR_OK      = 0,
  M24LR_ERROR   = 1,
  M24LR_BUSY    = 2,
  M24LR_TIMEOUT = 3
} M24LR_StatusTypeDef;

/**
 * @brief  M24LR VOUT Configuration enumerator definition
 */
typedef enum
{
  M24LR_EH_Cfg_6MA = 0,
  M24LR_EH_Cfg_3MA,
  M24LR_EH_Cfg_1MA,
  M24LR_EH_Cfg_300UA
} M24LR_EH_CFG_VOUT;

/**
 * @brief  M24LR FIELD status enumerator definition
 */
typedef enum
{
  M24LR_FIELD_OFF = 0,
  M24LR_FIELD_ON
} M24LR_FIELD_STATUS;

/**
 * @brief  M24LR TT-PROG status enumerator definition
 */
typedef enum
{
  M24LR_T_PROG_NO = 0,
  M24LR_T_PROG_OK
} M24LR_T_PROG_STATUS;

/**
 * @brief  M24LR Energy Harvesting status enumerator definition
 */
typedef enum
{
  M24LR_EH_DISABLE = 0,
  M24LR_EH_ENABLE
} M24LR_EH_STATUS;

/**
 * @brief  M24LR Energy Harvesting mode enumerator definition
 */
typedef enum
{
  M24LR_EH_MODE_ENABLE = 0,
  M24LR_EH_MODE_DISABLE
} M24LR_EH_MODE_STATUS;

/**
 * @brief  M24LR GPO status enumerator definition
 */
typedef enum
{
  M24LR_GPO_BUSY = 0,
  M24LR_GPO_WIP
} M24LR_GPO_STATUS;

/**
 * @brief  M24LR Memory information structure definition
 */
typedef struct
{
  uint8_t BlockSize;
  uint16_t Mem_Size;
} M24LR_Mem_Size;

/**
 * @brief  M24LR I2C Write Lock register structure definition
 */
typedef struct
{
  uint8_t sectors_7_0;
  uint8_t sectors_15_8;
  uint8_t sectors_23_16;
  uint8_t sectors_31_24;
  uint8_t sectors_39_32;
  uint8_t sectors_47_40;
  uint8_t sectors_55_48;
  uint8_t sectors_63_56;
} M24LR_Lock_Sectors;

/**
 * @brief  M24LR UID information structure definition
 */
typedef struct
{
  uint32_t MSB_UID;
  uint32_t LSB_UID;
} M24LR_UID;

/**
 * @brief  M24LR Sector Security register structure definition
 */
typedef struct
{
  uint8_t SectorLock;
  uint8_t RW_Protection;
  uint8_t PassCtrl;
} M24LR_SECTOR_SEC;

/**
 * @brief  M24LR_ driver structure definition
 */
typedef struct
{
  M24LR_StatusTypeDef       (*Init)( void );
  M24LR_StatusTypeDef       (*ReadID)( uint8_t * const );
  M24LR_StatusTypeDef       (*IsReady)( const uint32_t );
  M24LR_StatusTypeDef       (*ConfigIT)( const uint16_t );
  M24LR_StatusTypeDef       (*GetITStatus)( uint16_t * const );
  M24LR_StatusTypeDef       (*ReadData)( uint8_t * const, const uint16_t, const uint16_t );
  M24LR_StatusTypeDef       (*WriteData)( const uint8_t * const, const uint16_t, const uint16_t );
  M24LR_StatusTypeDef       (*ReadRegister)( uint8_t * const, const uint16_t, const uint16_t );
  M24LR_StatusTypeDef       (*WriteRegister)( const uint8_t * const, const uint16_t, const uint16_t );
  void                       *pData;
} M24LR_DrvTypeDef;

/**
 * @brief  M24LR_ extended driver structure definition
 */
typedef struct
{
  M24LR_StatusTypeDef (*ReadUID)( M24LR_UID * const );
  M24LR_StatusTypeDef (*ReadDSFID)( uint8_t * const );
  M24LR_StatusTypeDef (*ReadAFI)( uint8_t * const );
  M24LR_StatusTypeDef (*ReadI2CLockSector)( M24LR_Lock_Sectors * const );
  M24LR_StatusTypeDef (*I2CLockSector)( const uint8_t );
  M24LR_StatusTypeDef (*I2CUnlockSector)( const uint8_t );
  M24LR_StatusTypeDef (*PresentI2CPassword)( const uint32_t );
  M24LR_StatusTypeDef (*WriteI2CPassword)( const uint32_t );
  M24LR_StatusTypeDef (*ReadSSSx)( const uint8_t, M24LR_SECTOR_SEC * const );
  M24LR_StatusTypeDef (*WriteSSSx)( const uint8_t, const M24LR_SECTOR_SEC * const );
  M24LR_StatusTypeDef (*ReadMemSize)( M24LR_Mem_Size * const );
  M24LR_StatusTypeDef (*GetRF_WIP_BUSY)( M24LR_GPO_STATUS * const );
  M24LR_StatusTypeDef (*SetRFBUSY)( void );
  M24LR_StatusTypeDef (*SetRFWIP)( void );
  M24LR_StatusTypeDef (*Read_EH_mode)( M24LR_EH_MODE_STATUS * const );
  M24LR_StatusTypeDef (*Enable_EH_mode)( void );
  M24LR_StatusTypeDef (*Disable_EH_mode)( void );
  M24LR_StatusTypeDef (*ReadEH_Cfg)( M24LR_EH_CFG_VOUT * const );
  M24LR_StatusTypeDef (*WriteEH_Cfg)( const M24LR_EH_CFG_VOUT );
  M24LR_StatusTypeDef (*GetEH)( M24LR_EH_STATUS * const );
  M24LR_StatusTypeDef (*SetEH)( void );
  M24LR_StatusTypeDef (*ResetEH)( void );
  M24LR_StatusTypeDef (*GetRFField)( M24LR_FIELD_STATUS * const );
  M24LR_StatusTypeDef (*GetTProg)( M24LR_T_PROG_STATUS * const );
} M24LR_ExtDrvTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup M24LR_Exported_Constants
  * @{
  */
#define I_AM_M24LR04            0x5A
#define I_AM_M24LR16            0x4E
#define I_AM_M24LR64            0x5E

#ifndef NULL
#define NULL      (void *) 0
#endif

#define M24LR_PAGEWRITE_NBBYTE  4

#define M24LR_ADDR_DATA_I2C     0xA6
#define M24LR_ADDR_SYST_I2C     0xAE
#define M24LR_I2C_TIMEOUT       200 /* I2C Time out (ms), this is the maximum time needed by M24LR to complete any command */

#define M24LR_IT_BUSY_MASK      0x01
#define M24LR_IT_WIP_MASK       0x02

/* Registers address */
#define M24LR_SSS_REG           0x0000
#define M24LR_LOCK_REG          0x0800
#define M24LR_I2C_PWD_REG       0x0900
#define M24LR_CFG_REG           0x0910
#define M24LR_AFI_REG           0x0912
#define M24LR_DSFID_REG         0x0913
#define M24LR_UID_REG           0x0914
#define M24LR_ICREF_REG         0x091C
#define M24LR_MEMSIZE_REG       0x091D
#define M24LR_CTRL_REG          0x0920

/* Registers mask */
#define M24LR_SSS_LOCK_MASK     0x01
#define M24LR_SSS_RW_MASK       0x06
#define M24LR_SSS_PASSCTRL_MASK 0x18
#define M24LR_LOCK_MASK         0x0F

#define M24LR_CFG_EHCFG0_MASK   0x01
#define M24LR_CFG_EHCFG1_MASK   0x02
#define M24LR_CFG_EHMODE_MASK   0x04
#define M24LR_CFG_WIPBUSY_MASK  0x08

#define M24LR_CTRL_EHEN_MASK    0x01
#define M24LR_CTRL_FIELD_MASK   0x02
#define M24LR_CTRL_TPROG_MASK   0x80
/**
  * @}
  */

/* External variables --------------------------------------------------------*/
/** @addtogroup M24LR_Imported_Variables
 * @{
 */
/* M24LR_ driver structure */
extern M24LR_DrvTypeDef M24lr_i2c_Drv;
extern M24LR_ExtDrvTypeDef M24lr_i2c_ExtDrv;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Imported functions ------------------------------------------------------- */
/** @addtogroup M24LR_Imported_Functions
 * @{
 */
extern M24LR_StatusTypeDef M24lr_IO_Init( void );
extern M24LR_StatusTypeDef M24lr_IO_MemWrite( const uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size );
extern M24LR_StatusTypeDef M24lr_IO_MemRead( uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size );
extern M24LR_StatusTypeDef M24lr_IO_IsDeviceReady( const uint8_t DevAddr, const uint32_t Trials );
/**
 * @}
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef _cplusplus
  }
#endif
#endif /* _M24LR_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
