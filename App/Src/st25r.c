
#include "st25r.h"

#if FTR_NFCREADER
#include <spi.h>

#include <stdbool.h>
#include <stdint.h>

#include "rfal_nfc.h"
#include "rfal_analogConfig.h"

#if defined(ST25R3911)
# include "st25r3911_interrupt.h"
#elif defined(ST25R3916B)
# include "st25r3916_irq.h"
#endif

#if OS_FREERTOS
# include "cmsis_os.h"
#endif


/*****************************************/
/*           Defines & Typedefs          */
/*****************************************/
typedef enum{
    ST25R_STATE_INIT                =  0,  /* Initialize state            */
    ST25R_STATE_TECHDETECT          =  1,  /* Technology Detection state  */
    ST25R_STATE_COLAVOIDANCE        =  2,  /* Collision Avoidance state   */
    ST25R_STATE_ACTIVATION          =  3,  /* Activation state            */
    ST25R_STATE_CONNECTED           =  4,  /* Fully connected state, execute inner commands */
    ST25R_STATE_DEACTIVATION        =  5   /* Deactivation state          */
} ST25R_state;

#define ST25R_MAX_RETRIES   50       // Number of times to retry the send command
#define ST25R_RF_BUF_LEN    255   /* RF buffer length            */

// Device Definition
#define ST25R_NUM_DEVICES      10    /* Number of devices supported */

typedef rfalNfcvListenDevice ST25R_Device;

/*****************************************/
/*        Private Data Definitions       */
/*****************************************/
#define RX_BUF_SIZE 256

static const ST25R_command defaultCommand = {
    .cmd = NFC_COMMAND_GET_SYS_INFO
};

static struct {
    ST25R_state state;
    ReturnCode err;

    ST25R_Device devList[ST25R_NUM_DEVICES];
    uint8_t devCnt;
    ST25R_Device *activeDev;

    ST25R_command nextCommand;
    uint8_t retries;

    uint8_t rxBuf[RX_BUF_SIZE];
} st25r;

/*****************************************/
/*        Public Data Definitions        */
/*****************************************/
uint8_t globalCommProtectCnt = 0;

/*****************************************/
/*           Private Functions           */
/*****************************************/

static bool ST25R_TechDetection( void )
{
    rfalNfcvInventoryRes invRes;

    rfalNfcvPollerInitialize();                                                       /* Initialize RFAL for NFC-V */
    rfalFieldOnAndStartGT();                                                          /* As field is already On only starts GT timer */

    st25r.err = rfalNfcvPollerCheckPresence( &invRes );                                     /* Poll for NFC-V devices */
    return st25r.err == RFAL_ERR_NONE;
}

static bool ST25R_CollResolution( void )
{
    uint8_t    i;
    uint8_t    devCnt;
    ReturnCode err;

    /*******************************************************************************/
    /* NFC-V Collision Resolution                                                  */
    /*******************************************************************************/
    rfalNfcvListenDevice nfcvDevList[ST25R_NUM_DEVICES];

    rfalNfcvPollerInitialize();
    rfalFieldOnAndStartGT();                                                      /* Ensure GT again as other technologies have also been polled */
    err = rfalNfcvPollerCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, (ST25R_NUM_DEVICES - st25r.devCnt), nfcvDevList, &devCnt );
    if( (err == RFAL_ERR_NONE) && (devCnt != 0) )
    {
        for( i=0; i<devCnt; i++ )                                                /* Copy devices found form local Nfcf list into global device list */
        {
            st25r.devList[st25r.devCnt] = nfcvDevList[i];
            st25r.devCnt++;
        }
    }

    return (st25r.devCnt > 0);
}

static bool ST25R_Activation( uint8_t devIt )
{
    ReturnCode           err;
    rfalNfcbSensbRes     sensbRes;
    uint8_t              sensbResLen;

    if( devIt > st25r.devCnt )
    {
        return false;
    }

    rfalNfcvPollerInitialize();

    /* No specific activation needed for a T5T */
    platformLog("NFC-V T5T device activated \r\n");                           /* NFC-V T5T device activated */

    st25r.activeDev = &st25r.devList[devIt];                                                    /* Assign active device to be used further on */
    return true;
}

static ReturnCode ST25R_Connected( void )
{
    extern osMessageQueueId_t nfcCommandQueueHandle;
    ReturnCode ret;

    // Check if we have a command, if not, send a empty command to check if its still here
    // Include a delay to ensure that while we don't have a pending command, we allow the I2C operation to run
    if (osMessageQueueGet(nfcCommandQueueHandle, &st25r.nextCommand, NULL, 200) != osOK)
    {
        memcpy(&st25r.nextCommand, &defaultCommand, sizeof(defaultCommand));
    }

    // We received a new command, reset our retries
    st25r.retries = ST25R_MAX_RETRIES;

    uint16_t rcvLen;

    while (st25r.retries > 0)
    // while(1)
    {
        switch (st25r.nextCommand.cmd)
        {
            case NFC_COMMAND_READ_SINGLE_BLOCK:
                ret = rfalNfcvPollerReadSingleBlock(
                    RFAL_NFCV_REQ_FLAG_DEFAULT,
                    NULL,
                    st25r.nextCommand.readSingleCmd.addr,
                    st25r.rxBuf,
                    RX_BUF_SIZE,
                    &rcvLen
                );

                memcpy(
                    st25r.nextCommand.readSingleCmd.rxLoc,
                    &st25r.rxBuf[1],
                    4U
                );

                break;

            case NFC_COMMAND_READ_MULTIPLE_BLOCK:
                ret = rfalNfcvPollerReadMultipleBlocks(
                    RFAL_NFCV_REQ_FLAG_DEFAULT,
                    NULL,
                    st25r.nextCommand.readMultipleCmd.addr,
                    st25r.nextCommand.readMultipleCmd.len,
                    st25r.rxBuf,
                    RX_BUF_SIZE,
                    &rcvLen
                );

                memcpy(
                    st25r.nextCommand.readMultipleCmd.rxLoc,
                    &st25r.rxBuf[1],
                    4U * (st25r.nextCommand.readMultipleCmd.len + 1)
                );

                break;

            case NFC_COMMAND_WRITE_SINGLE_BLOCK:
                ret = rfalNfcvPollerWriteSingleBlock(
                    RFAL_NFCV_REQ_FLAG_DEFAULT,
                    NULL,
                    st25r.nextCommand.writeSingleCmd.addr,
                    (uint8_t *) &st25r.nextCommand.writeSingleCmd.data,
                    4U
                );

                break;

            case NFC_COMMAND_GET_SYS_INFO:
                ret = rfalNfcvPollerGetSystemInformation(
                    RFAL_NFCV_REQ_FLAG_DEFAULT,
                    NULL,
                    st25r.rxBuf,
                    RX_BUF_SIZE,
                    &rcvLen
                );

            default:
                // Unimplemented command

                break;
        }

        if (ret == RFAL_ERR_NONE)
            return ret;
        else
            st25r.retries--;

        // Blocking wait between retries.
        HAL_Delay(10);
    }

    return ret;
}

static bool ST25R_Deactivate( void )
{
    return true;
}

/*****************************************/
/*           Public Functions            */
/*****************************************/
void ST25R_task(void *arg)
{
    (void) arg;

    rfalAnalogConfigInitialize();                                                     /* Initialize RFAL's Analog Configs */
    rfalInitialize();

    for (;;)
    {
        rfalWorker();

        switch (st25r.state)
        {
            case ST25R_STATE_INIT:
                st25r.activeDev  = NULL;
                st25r.devCnt     = 0;
                memset(&st25r.nextCommand, 0U, sizeof(ST25R_command));

                st25r.state = ST25R_STATE_TECHDETECT;
                break;

            case ST25R_STATE_TECHDETECT:

                if( !ST25R_TechDetection() )                             /* Poll for nearby devices in different technologies */
                {
                    st25r.state = ST25R_STATE_DEACTIVATION;                  /* If no device was found, restart loop */
                    break;
                }

                st25r.state = ST25R_STATE_COLAVOIDANCE;                      /* One or more devices found, go to Collision Avoidance */
                break;

            case ST25R_STATE_COLAVOIDANCE:

                if( !ST25R_CollResolution() )                              /* Resolve any eventual collision */
                {
                    st25r.state = ST25R_STATE_DEACTIVATION;                  /* If Collision Resolution was unable to retrieve any device, restart loop */
                    break;
                }

                st25r.state = ST25R_STATE_ACTIVATION;                        /* Device(s) have been identified, go to Activation */
                break;

            case ST25R_STATE_ACTIVATION:

                if( !ST25R_Activation( 0 ) )                               /* Any device previous identified can be Activated, on this example will select the firt on the list */
                {
                    st25r.state = ST25R_STATE_DEACTIVATION;                  /* If Activation failed, restart loop */
                    break;
                }

                st25r.state = ST25R_STATE_CONNECTED; /* Device has been properly activated, we are now connected */
                break;

            case ST25R_STATE_CONNECTED:

                st25r.err = ST25R_Connected();
                switch( st25r.err )
                {
                    case RFAL_ERR_NONE: // No issues, stay connected
                            st25r.state = ST25R_STATE_CONNECTED;
                            break;

                    default:                                                          /* Data exchange not successful, card removed or other transmission error */
                        st25r.state = ST25R_STATE_DEACTIVATION;              /* Restart loop */
                        break;
                }
                break;

            case ST25R_STATE_DEACTIVATION:

                ST25R_Deactivate();                                        /* If a card has been activated, properly deactivate the device */

                rfalFieldOff();                                                       /* Turn the Field Off powering down any device nearby */
                platformDelay( 500 );                                                 /* Remain a certain period with field off */

                st25r.state = ST25R_STATE_INIT;                              /* Restart the loop */
                break;

            default:
                break;
        }

        if (st25r.state == ST25R_STATE_INIT)
        {
            platformDelay(5);
        }
    }
}

bool ST25R_connected()
{
    return (
        (st25r.state == ST25R_STATE_CONNECTED)
    );
}

// INTR Callback function
void ST25R_EXTICallback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ST25R_INT_PIN)
    {
#if defined(ST25R3911)
        st25r3911Isr();
#elif defined(ST25R3916B)
        // st25r3916Isr();
#endif
    }
}
#endif

#if defined(ST25R3916B)
// We have to define some I2C functions
# include <i2c.h>

int32_t ST25R_SequentialSend(const uint16_t DevAddr, const uint8_t * const pData, const uint16_t Length, const uint8_t last, const uint8_t txOnly)
{
    HAL_StatusTypeDef ret;
    uint32_t txFlag;

    if ((last != 0) && (txOnly != 0))
    {
      txFlag = I2C_LAST_FRAME;
    }
    else
    {
      txFlag = (last ? /*I2C_LAST_FRAME_NO_STOP*/ I2C_FIRST_FRAME : I2C_FIRST_AND_NEXT_FRAME);
    }

    ret = HAL_I2C_Master_Seq_Transmit_IT(&ST25R_I2C, DevAddr, (uint8_t*)pData, Length, txFlag);
    if (ret != HAL_OK)
    {
      return ret;
    }

    while (HAL_I2C_GetState(&ST25R_I2C) != HAL_I2C_STATE_READY);

    if (HAL_I2C_GetError(&ST25R_I2C) != HAL_I2C_ERROR_NONE)
    {
      return HAL_ERROR;
    }
    return HAL_OK;
}

int32_t ST25R_SequentialReceive(const uint16_t DevAddr, uint8_t * const pData, uint16_t Length)
{
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Seq_Receive_IT(&ST25R_I2C, DevAddr, (uint8_t*)pData, Length, I2C_LAST_FRAME);
    if (ret != HAL_OK)
    {
      return ret;
    }

    while (HAL_I2C_GetState(&ST25R_I2C) != HAL_I2C_STATE_READY);

    if (HAL_I2C_GetError(&ST25R_I2C) != HAL_I2C_ERROR_NONE)
    {
      return HAL_ERROR;
    }

    return HAL_OK;
}

#endif
