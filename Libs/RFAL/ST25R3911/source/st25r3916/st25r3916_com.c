
/******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 STMicroelectronics, all rights reserved
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/


/*
 *      PROJECT:   ST25R3916 firmware
 *      Revision:
 *      LANGUAGE:  ISO C99
 */

/*! \file
 *
 *  \author Gustavo Patricio
 *
 *  \brief Implementation of ST25R3916 communication
 *
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include "st25r3916.h"
#include "st25r3916_com.h"
#include "st25r3916_led.h"
#include "rfal_platform.h"
#include "rfal_utils.h"
#include <i2c.h>


/*
******************************************************************************
* LOCAL DEFINES
******************************************************************************
*/

#define ST25R3916_OPTIMIZE              true                           /*!< Optimization switch: false always write value to register      */
#define ST25R3916_I2C_ADDR              (0xA0U)                   /*!< ST25R3916's default I2C address                                */
#define ST25R3916_REG_LEN               1U                             /*!< Byte length of a ST25R3916 register                            */
#define ST25R3916_MOSI_IDLE             (0x00)                         /*!< ST25R3916 MOSI IDLE state                                      */

#define ST25R3916_WRITE_MODE            (0U << 6)                      /*!< ST25R3916 Operation Mode: Write                                */
#define ST25R3916_READ_MODE             (1U << 6)                      /*!< ST25R3916 Operation Mode: Read                                 */
#define ST25R3916_CMD_MODE              (3U << 6)                      /*!< ST25R3916 Operation Mode: Direct Command                       */
#define ST25R3916_FIFO_LOAD             (0x80U)                        /*!< ST25R3916 Operation Mode: FIFO Load                            */
#define ST25R3916_FIFO_READ             (0x9FU)                        /*!< ST25R3916 Operation Mode: FIFO Read                            */
#define ST25R3916_PT_A_CONFIG_LOAD      (0xA0U)                        /*!< ST25R3916 Operation Mode: Passive Target Memory A-Config Load  */
#define ST25R3916_PT_F_CONFIG_LOAD      (0xA8U)                        /*!< ST25R3916 Operation Mode: Passive Target Memory F-Config Load  */
#define ST25R3916_PT_TSN_DATA_LOAD      (0xACU)                        /*!< ST25R3916 Operation Mode: Passive Target Memory TSN Load       */
#define ST25R3916_PT_MEM_READ           (0xBFU)                        /*!< ST25R3916 Operation Mode: Passive Target Memory Read           */

#define ST25R3916_CMD_LEN               (1U)                           /*!< ST25R3916 CMD length                                           */
#define ST25R3916_BUF_LEN               (ST25R3916_CMD_LEN+ST25R3916_FIFO_DEPTH) /*!< ST25R3916 communication buffer: CMD + FIFO length    */

/*
******************************************************************************
* MACROS
******************************************************************************/

#if defined(ST25R_COM_SINGLETXRX) && !defined(RFAL_USE_I2C)
static uint8_t  comBuf[ST25R3916_BUF_LEN];                             /*!< ST25R3916 communication buffer                                 */
static uint16_t comBufIt;                                              /*!< ST25R3916 communication buffer iterator                        */
#endif /* ST25R_COM_SINGLETXRX */


/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/

/*******************************************************************************/
ReturnCode st25r3916ReadRegister( uint8_t reg, uint8_t* val )
{
    return st25r3916ReadMultipleRegisters( reg, val, ST25R3916_REG_LEN );
}


/*******************************************************************************/
ReturnCode st25r3916ReadMultipleRegisters( uint8_t reg, uint8_t* values, uint8_t length )
{
    if( length > 0U )
    {
        uint16_t TarAddr = ((reg & ~ST25R3916_SPACE_B) | ST25R3916_READ_MODE);
        uint16_t TarAddrSize = I2C_MEMADD_SIZE_8BIT;

        /* If is a space-B register send a direct command first */
        if( (reg & ST25R3916_SPACE_B) != 0U )
        {
            TarAddr |= ST25R3916_CMD_SPACE_B_ACCESS << 8U;
            TarAddrSize = I2C_MEMADD_SIZE_16BIT;
        }

        HAL_StatusTypeDef status = HAL_I2C_Mem_Read( &ST25R_I2C, ST25R3916_I2C_ADDR, TarAddr, TarAddrSize, values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916WriteRegister( uint8_t reg, uint8_t val )
{
    uint8_t value = val;               /* MISRA 17.8: use intermediate variable */
    return st25r3916WriteMultipleRegisters( reg, &value, ST25R3916_REG_LEN );
}


/*******************************************************************************/
ReturnCode st25r3916WriteMultipleRegisters( uint8_t reg, const uint8_t* values, uint8_t length )
{
    if( length > 0U )
    {
        uint16_t TarAddr = ((reg & ~ST25R3916_SPACE_B) | ST25R3916_WRITE_MODE);
        uint16_t TarAddrSize = I2C_MEMADD_SIZE_8BIT;

        /* If is a space-B register send a direct command first */
        if( (reg & ST25R3916_SPACE_B) != 0U )
        {
            TarAddr |= ST25R3916_CMD_SPACE_B_ACCESS << 8U;
            TarAddrSize = I2C_MEMADD_SIZE_16BIT;
        }

        HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, TarAddr, TarAddrSize, (uint8_t *)values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916WriteFifo( const uint8_t* values, uint16_t length )
{
    if( length > ST25R3916_FIFO_DEPTH )
    {
        return RFAL_ERR_PARAM;
    }

    if( length > 0U )
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_FIFO_LOAD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916ReadFifo( uint8_t* buf, uint16_t length )
{
    if( length > 0U )
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Read( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_FIFO_READ, I2C_MEMADD_SIZE_8BIT, buf, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916WritePTMem( const uint8_t* values, uint16_t length )
{
    if( length > ST25R3916_PTM_LEN )
    {
        return RFAL_ERR_PARAM;
    }

    if( length > 0U )
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_PT_A_CONFIG_LOAD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916ReadPTMem( uint8_t* values, uint16_t length )
{
    uint8_t tmp[ST25R3916_REG_LEN + ST25R3916_PTM_LEN];  /* local buffer to handle prepended byte on I2C and SPI */

    if( length > 0U )
    {
        if( length > ST25R3916_PTM_LEN )
        {
            return RFAL_ERR_PARAM;
        }

        HAL_StatusTypeDef status = HAL_I2C_Mem_Read( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_PT_MEM_READ, I2C_MEMADD_SIZE_8BIT, tmp, ST25R3916_REG_LEN + length, ST25R_I2C_TIMEOUT );

        /* Copy PTMem content without prepended byte */
        RFAL_MEMCPY( values, (tmp+ST25R3916_REG_LEN), length );

        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916WritePTMemF( const uint8_t* values, uint16_t length )
{
    if( length > (ST25R3916_PTM_F_LEN + ST25R3916_PTM_TSN_LEN) )
    {
        return RFAL_ERR_PARAM;
    }

    if( length > 0U )
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_PT_F_CONFIG_LOAD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916WritePTMemTSN( const uint8_t* values, uint16_t length )
{
    if( length > ST25R3916_PTM_TSN_LEN )
    {
        return RFAL_ERR_PARAM;
    }

    if(length > 0U)
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, ST25R3916_PT_TSN_DATA_LOAD, I2C_MEMADD_SIZE_8BIT, (uint8_t *)values, length, ST25R_I2C_TIMEOUT );
        return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
    }

    return RFAL_ERR_NONE;
}


/*******************************************************************************/
ReturnCode st25r3916ExecuteCommand( uint8_t cmd )
{

    cmd |= ST25R3916_CMD_MODE;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit( &ST25R_I2C, ST25R3916_I2C_ADDR, &cmd, 1U, ST25R_I2C_TIMEOUT );
    return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
}


/*******************************************************************************/
ReturnCode st25r3916ReadTestRegister( uint8_t reg, uint8_t* val )
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read( &ST25R_I2C, ST25R3916_I2C_ADDR, (ST25R3916_CMD_TEST_ACCESS << 8U) | (reg | ST25R3916_READ_MODE), I2C_MEMADD_SIZE_16BIT, val, ST25R3916_REG_LEN, ST25R_I2C_TIMEOUT );
    return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
}


/*******************************************************************************/
ReturnCode st25r3916WriteTestRegister( uint8_t reg, uint8_t val )
{
    uint8_t value = val;               /* MISRA 17.8: use intermediate variable */

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &ST25R_I2C, ST25R3916_I2C_ADDR, (ST25R3916_CMD_TEST_ACCESS << 8U) | (reg | ST25R3916_WRITE_MODE), I2C_MEMADD_SIZE_16BIT, &value, ST25R3916_REG_LEN, ST25R_I2C_TIMEOUT );
    return status == HAL_OK ? RFAL_ERR_NONE : RFAL_ERR_SYSTEM;
}


/*******************************************************************************/
ReturnCode st25r3916ClrRegisterBits( uint8_t reg, uint8_t clr_mask )
{
    ReturnCode ret;
    uint8_t    rdVal;

    /* Read current reg value */
    RFAL_EXIT_ON_ERR( ret, st25r3916ReadRegister(reg, &rdVal) );

    /* Only perform a Write if value to be written is different */
    if( ST25R3916_OPTIMIZE && (rdVal == (uint8_t)(rdVal & ~clr_mask)) )
    {
        return RFAL_ERR_NONE;
    }

    /* Write new reg value */
    return st25r3916WriteRegister(reg, (uint8_t)(rdVal & ~clr_mask) );
}


/*******************************************************************************/
ReturnCode st25r3916SetRegisterBits( uint8_t reg, uint8_t set_mask )
{
    ReturnCode ret;
    uint8_t    rdVal;

    /* Read current reg value */
    RFAL_EXIT_ON_ERR( ret, st25r3916ReadRegister(reg, &rdVal) );

    /* Only perform a Write if the value to be written is different */
    if( ST25R3916_OPTIMIZE && (rdVal == (rdVal | set_mask)) )
    {
        return RFAL_ERR_NONE;
    }

    /* Write new reg value */
    return st25r3916WriteRegister(reg, (rdVal | set_mask) );
}


/*******************************************************************************/
ReturnCode st25r3916ChangeRegisterBits( uint8_t reg, uint8_t valueMask, uint8_t value )
{
    return st25r3916ModifyRegister(reg, valueMask, (valueMask & value) );
}


/*******************************************************************************/
ReturnCode st25r3916ModifyRegister( uint8_t reg, uint8_t clr_mask, uint8_t set_mask )
{
    ReturnCode ret;
    uint8_t    rdVal;
    uint8_t    wrVal;

    /* Read current reg value */
    RFAL_EXIT_ON_ERR( ret, st25r3916ReadRegister(reg, &rdVal) );

    /* Compute new value */
    wrVal  = (uint8_t)(rdVal & ~clr_mask);
    wrVal |= set_mask;

    /* Only perform a Write if the value to be written is different */
    if( ST25R3916_OPTIMIZE && (rdVal == wrVal) )
    {
        return RFAL_ERR_NONE;
    }

    /* Write new reg value */
    return st25r3916WriteRegister(reg, wrVal );
}


/*******************************************************************************/
ReturnCode st25r3916ChangeTestRegisterBits( uint8_t reg, uint8_t valueMask, uint8_t value )
{
    ReturnCode ret;
    uint8_t    rdVal;
    uint8_t    wrVal;

    /* Read current reg value */
    RFAL_EXIT_ON_ERR( ret, st25r3916ReadTestRegister(reg, &rdVal) );

    /* Compute new value */
    wrVal  = (uint8_t)(rdVal & ~valueMask);
    wrVal |= (uint8_t)(value & valueMask);

    /* Only perform a Write if the value to be written is different */
    if( ST25R3916_OPTIMIZE && (rdVal == wrVal) )
    {
        return RFAL_ERR_NONE;
    }

    /* Write new reg value */
    return st25r3916WriteTestRegister(reg, wrVal );
}


/*******************************************************************************/
bool st25r3916CheckReg( uint8_t reg, uint8_t mask, uint8_t val )
{
    uint8_t regVal;

    regVal = 0;
    st25r3916ReadRegister( reg, &regVal );

    return ( (regVal & mask) == val );
}


/*******************************************************************************/
bool st25r3916IsRegValid( uint8_t reg )
{
    if( !(( (int16_t)reg >= (int16_t)ST25R3916_REG_IO_CONF1) && (reg <= (ST25R3916_SPACE_B | ST25R3916_REG_IC_IDENTITY)) ))
    {
        return false;
    }
    return true;
}
