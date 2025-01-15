
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
 *      PROJECT:   ST25R3911 firmware
 *      Revision:
 *      LANGUAGE:  ISO C99
 */

/*! \file
 *
 *  \author Ulrich Herrmann
 *
 *  \brief Implementation of ST25R3911 communication.
 *
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include "st25r3911_com.h"
#include "st25r3911.h"
#include "rfal_utils.h"


/*
******************************************************************************
* LOCAL DEFINES
******************************************************************************
*/

#define ST25R3911_WRITE_MODE  (0U)                           /*!< ST25R3911 SPI Operation Mode: Write                            */
#define ST25R3911_READ_MODE   (1U << 6)                      /*!< ST25R3911 SPI Operation Mode: Read                             */
#define ST25R3911_FIFO_LOAD   (2U << 6)                      /*!< ST25R3911 SPI Operation Mode: FIFO Load                        */
#define ST25R3911_FIFO_READ   (0xBFU)                        /*!< ST25R3911 SPI Operation Mode: FIFO Read                        */
#define ST25R3911_CMD_MODE    (3U << 6)                      /*!< ST25R3911 SPI Operation Mode: Direct Command                   */

#define ST25R3911_CMD_LEN     (1U)                           /*!< ST25R3911 CMD length                                           */
#define ST25R3911_BUF_LEN     (ST25R3911_CMD_LEN+ST25R3911_FIFO_DEPTH)  /*!< ST25R3911 communication buffer: CMD + FIFO length   */

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/

#ifdef ST25R_COM_SINGLETXRX
static uint8_t comBuf[ST25R3911_BUF_LEN];    /*!< ST25R3911 communication buffer            */
#endif /* ST25R_COM_SINGLETXRX */

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

static inline void st25r3911CheckFieldSetLED(uint8_t value)
{
    if ((ST25R3911_REG_OP_CONTROL_tx_en & value) != 0U)
    {
#ifdef PLATFORM_LED_FIELD_PIN
        platformLedOn( PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN );
    }
    else
    {
        platformLedOff( PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN );
#endif /* PLATFORM_LED_FIELD_PIN */
    }
}


/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/
#include "spi.h"
void st25r3911ReadRegister(uint8_t reg, uint8_t* value)
{

    platformProtectST25RComm();

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    uint8_t pattern = ST25R3911_READ_MODE | reg;
    HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

    // Read the data
    uint8_t data;
    HAL_SPI_Receive(&ST25R_SPI, &data, 1U, HAL_MAX_DELAY);

    if(value != NULL)
    {
        *value = data;
    }

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);

    platformUnprotectST25RComm();
}

void st25r3911ReadMultipleRegisters(uint8_t reg, uint8_t* values, uint8_t length)
{
    if (length > 0U)
    {
        for (uint8_t i = 0; i < length; i++)
        {
            st25r3911ReadRegister(reg + i, &values[i]);
        }
    }

    return;
}

void st25r3911ReadTestRegister(uint8_t reg, uint8_t* value)
{

#ifdef ST25R_COM_SINGLETXRX
    uint8_t* buf = comBuf;
#else  /* ST25R_COM_SINGLETXRX */
    uint8_t  buf[3];
#endif  /* ST25R_COM_SINGLETXRX */

    platformProtectST25RComm();
    platformSpiSelect();

    buf[0] = ST25R3911_CMD_TEST_ACCESS;
    buf[1] = (reg | ST25R3911_READ_MODE);
    buf[2] = 0x00;

    HAL_SPI_Transmit(&ST25R_SPI, buf, 2U, HAL_MAX_DELAY);
    HAL_SPI_Receive(&ST25R_SPI, &buf[2], 1U, HAL_MAX_DELAY);

    if(value != NULL)
    {
      *value = buf[2];
    }

    platformSpiDeselect();
    platformUnprotectST25RComm();

    return;
}

void st25r3911WriteTestRegister(uint8_t reg, uint8_t value)
{
#ifdef ST25R_COM_SINGLETXRX
    uint8_t* buf = comBuf;
#else  /* ST25R_COM_SINGLETXRX */
    uint8_t  buf[3];
#endif  /* ST25R_COM_SINGLETXRX */

    platformProtectST25RComm();
    platformSpiSelect();

    buf[0] = ST25R3911_CMD_TEST_ACCESS;
    buf[1] = (reg | ST25R3911_WRITE_MODE);
    buf[2] = value;

    HAL_SPI_Transmit(&ST25R_SPI, buf, 3U, HAL_MAX_DELAY);

    platformSpiDeselect();
    platformUnprotectST25RComm();

    return;
}

void st25r3911WriteRegister(uint8_t reg, uint8_t value)
{
    st25r3911WriteMultipleRegisters(reg, &value, 1U);

    return;
}

void st25r3911ClrRegisterBits( uint8_t reg, uint8_t clr_mask )
{
    uint8_t tmp;

    st25r3911ReadRegister(reg, &tmp);
    tmp &= ~clr_mask;
    st25r3911WriteRegister(reg, tmp);

    return;
}


void st25r3911SetRegisterBits( uint8_t reg, uint8_t set_mask )
{
    uint8_t tmp;

    st25r3911ReadRegister(reg, &tmp);
    tmp |= set_mask;
    st25r3911WriteRegister(reg, tmp);

    return;
}

void st25r3911ChangeRegisterBits(uint8_t reg, uint8_t valueMask, uint8_t value)
{
    st25r3911ModifyRegister(reg, valueMask, (valueMask & value) );
}

void st25r3911ModifyRegister(uint8_t reg, uint8_t clr_mask, uint8_t set_mask)
{
    uint8_t tmp;

    st25r3911ReadRegister(reg, &tmp);

    /* mask out the bits we don't want to change */
    tmp &= ~clr_mask;
    /* set the new value */
    tmp |= set_mask;
    st25r3911WriteRegister(reg, tmp);

    return;
}

void st25r3911ChangeTestRegisterBits( uint8_t reg, uint8_t valueMask, uint8_t value )
{
    uint8_t    rdVal;
    uint8_t    wrVal;

    /* Read current reg value */
    st25r3911ReadTestRegister(reg, &rdVal);

    /* Compute new value */
    wrVal  = (rdVal & ~valueMask);
    wrVal |= (value & valueMask);

    /* Write new reg value */
    st25r3911WriteTestRegister(reg, wrVal );

    return;
}

void st25r3911WriteMultipleRegisters(uint8_t reg, const uint8_t* values, uint8_t length)
{

    platformProtectST25RComm();

    if (length > 0U)
    {
        HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

        // Write pattern
        uint8_t pattern = ST25R3911_WRITE_MODE | reg;
        HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

        // Write the values
        HAL_SPI_Transmit(&ST25R_SPI, values, length, HAL_MAX_DELAY);

        HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);
    }

    platformUnprotectST25RComm();

}

void st25r3911WriteFifo(const uint8_t* values, uint8_t length)
{
#if !defined(ST25R_COM_SINGLETXRX)
    const uint8_t cmd = ST25R3911_FIFO_LOAD;
#endif  /* !ST25R_COM_SINGLETXRX */

    if( (length > 0U) && (length <= ST25R3911_FIFO_DEPTH) )
    {
        platformProtectST25RComm();
        platformSpiSelect();

#ifdef ST25R_COM_SINGLETXRX

        comBuf[0] = ST25R3911_FIFO_LOAD;
        RFAL_MEMCPY( &comBuf[ST25R3911_CMD_LEN], values, RFAL_MIN( length, ST25R3911_BUF_LEN - ST25R3911_CMD_LEN ) );

        platformSpiTxRx( comBuf, NULL, RFAL_MIN( (ST25R3911_CMD_LEN + length), ST25R3911_BUF_LEN ) );

#else  /*ST25R_COM_SINGLETXRX*/

        HAL_SPI_Transmit(&ST25R_SPI, &cmd, 1U, HAL_MAX_DELAY);
        HAL_SPI_Transmit(&ST25R_SPI, values, length, HAL_MAX_DELAY);

#endif  /*ST25R_COM_SINGLETXRX*/

        platformSpiDeselect();
        platformUnprotectST25RComm();
    }

    return;
}

void st25r3911ReadFifo(uint8_t* buf, uint8_t length)
{
#if !defined(ST25R_COM_SINGLETXRX)
    const uint8_t cmd = ST25R3911_FIFO_READ;
#endif  /* !ST25R_COM_SINGLETXRX */

    if(length > 0U)
    {
        platformProtectST25RComm();
        platformSpiSelect();

#ifdef ST25R_COM_SINGLETXRX

        RFAL_MEMSET( comBuf, 0x00, RFAL_MIN( (ST25R3911_CMD_LEN + (uint32_t)length), ST25R3911_BUF_LEN ) );
        comBuf[0] = ST25R3911_FIFO_READ;

        platformSpiTxRx( comBuf, comBuf, RFAL_MIN( (ST25R3911_CMD_LEN + length), ST25R3911_BUF_LEN ) );          /* Transceive as a single SPI call                        */
        if( buf != NULL )
        {
            RFAL_MEMCPY( buf, &comBuf[ST25R3911_CMD_LEN], RFAL_MIN( length, ST25R3911_BUF_LEN - ST25R3911_CMD_LEN ) ); /* Copy from local buf to output buffer and skip cmd byte */
        }

#else  /*ST25R_COM_SINGLETXRX*/

        if( buf != NULL )
        {
            RFAL_MEMSET( buf, 0x00, length );
        }

        HAL_SPI_Transmit(&ST25R_SPI, &cmd, 1U, HAL_MAX_DELAY);
        HAL_SPI_Receive(&ST25R_SPI, buf, length, HAL_MAX_DELAY);

#endif  /*ST25R_COM_SINGLETXRX*/

        platformSpiDeselect();
        platformUnprotectST25RComm();
    }

    return;
}

void st25r3911ExecuteCommand( uint8_t cmd )
{
    platformProtectST25RComm();
    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    uint8_t pattern = ST25R3911_CMD_MODE | cmd;
    HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);
    platformUnprotectST25RComm();
    return;
}


void st25r3911ExecuteCommands(const uint8_t *cmds, uint8_t length)
{
    platformProtectST25RComm();
    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    HAL_SPI_Transmit(&ST25R_SPI, cmds, length, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);
    platformUnprotectST25RComm();

    return;
}

bool st25r3911IsRegValid( uint8_t reg )
{
    if( (!(( (int16_t)reg >= (int16_t)ST25R3911_REG_IO_CONF1) && (reg <= ST25R3911_REG_CAPACITANCE_MEASURE_RESULT))) &&  (reg != ST25R3911_REG_IC_IDENTITY)  )
    {
        return false;
    }
    return true;
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/

