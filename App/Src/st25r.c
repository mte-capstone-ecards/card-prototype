
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
    ST25R_STATE_DATAEXCHANGE_START  =  4,  /* Data Exchange Start state   */
    ST25R_STATE_DATAEXCHANGE_CHECK  =  5,  /* Data Exchange Check state   */
    ST25R_STATE_DEACTIVATION        =  9   /* Deactivation state          */
} ST25R_state;

#define ST25R_RF_BUF_LEN   255   /* RF buffer length            */

// Device Definition
#define ST25R_NUM_DEVICES      10    /* Number of devices supported */

typedef rfalNfcvListenDevice ST25R_Device;

#if !OS_FREERTOS
// static uint8_t nfcSelectedCommandPayload[] = ST25R_CMD_WRITE_SINGLE_BLOCK(0x00, 0xABCD0055);
static uint8_t nfcSelectedCommandPayload[] = ST25R_CMD_READ_SINGLE_BLOCK(0x00);
static ST25R_command nfcSelectedCommand = {
    .payload = nfcSelectedCommandPayload,
    .payloadSize = sizeof(nfcSelectedCommandPayload),
};
#endif

/*****************************************/
/*        Private Data Definitions       */
/*****************************************/

static struct {
    ST25R_state state;
    ReturnCode err;

    ST25R_Device devList[ST25R_NUM_DEVICES];
    uint8_t devCnt;

    ST25R_Device *activeDev;

    ST25R_command nextCommand;

    uint8_t rfTxBuf[ST25R_RF_BUF_LEN];
    uint8_t rfRxBuf[ST25R_RF_BUF_LEN];

    uint16_t rcvLen;
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

static ReturnCode ST25R_DataExchange( void )
{
    rfalTransceiveContext ctx;
    ReturnCode            err;
    rfalIsoDepTxRxParam   isoDepTxRx;
    rfalNfcDepTxRxParam   nfcDepTxRx;
    uint8_t               *txBuf;
    uint16_t              txBufLen;


    /*******************************************************************************/
    /* The Data Exchange is divided in two different moments, the trigger/Start of *
     *  the transfer followed by the check until its completion                    */
    if( st25r.state == ST25R_STATE_DATAEXCHANGE_START )                      /* Trigger/Start the data exchange */
    {
        // Check if the message exists
        if (st25r.nextCommand.payloadSize == 0U)
        {
            return RFAL_ERR_NOMSG;
        }

        txBuf    = st25r.nextCommand.payload;
        txBufLen = st25r.nextCommand.payloadSize;

        /*******************************************************************************/
        /* Trigger a RFAL Transceive using the previous defined frames                 */
        rfalCreateByteFlagsTxRxContext( ctx, txBuf, txBufLen, st25r.rfRxBuf, sizeof(st25r.rfRxBuf), &st25r.rcvLen, RFAL_TXRX_FLAGS_DEFAULT, rfalConvMsTo1fc(20) );
        return (((err = rfalStartTransceive( &ctx )) == RFAL_ERR_NONE) ? RFAL_ERR_BUSY : err);     /* Signal RFAL_ERR_BUSY as Data Exchange has been started and is ongoing */
    }
    /*******************************************************************************/
    /* The Data Exchange has been started, wait until completed                    */
    else if( st25r.state == ST25R_STATE_DATAEXCHANGE_CHECK )
    {
        return rfalGetTransceiveStatus();
    }
    return RFAL_ERR_REQUEST;
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

                st25r.state = ST25R_STATE_DATAEXCHANGE_START;                /* Device has been properly activated, go to Data Exchange */
                break;

            case ST25R_STATE_DATAEXCHANGE_START:
            case ST25R_STATE_DATAEXCHANGE_CHECK:

                st25r.err = ST25R_DataExchange();                                /* Perform Data Exchange, in this example a simple transfer will executed in order to do device's presence check */
                switch( st25r.err )
                {
                    case RFAL_ERR_NONE:                                                    /* Data exchange successful  */
                        memset(&st25r.nextCommand, 0U, sizeof(ST25R_command));  // Clear the previous command
                        st25r.state = ST25R_STATE_DATAEXCHANGE_START;        /* Trigger new exchange with device */
                        break;

                    case RFAL_ERR_BUSY:                                                    /* Data exchange ongoing  */
                        st25r.state = ST25R_STATE_DATAEXCHANGE_CHECK;        /* Once triggered/started the Data Exchange only do check until is completed */
                        break;

                    case RFAL_ERR_NOMSG:                                                   /* No message to send. Grab a message from the queue  */
                        extern osMessageQueueId_t nfcCommandQueueHandle;
                        osStatus_t status = osMessageQueueGet(nfcCommandQueueHandle, &st25r.nextCommand, NULL, 10);
                        st25r.state = ST25R_STATE_DATAEXCHANGE_START;        /* Trigger new exchange with device */
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
            return;
        }

        if (st25r.state == ST25R_STATE_INIT)
        {
            platformDelay(5);
        }
    }
}

// INTR Callback function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ST25R_INT_PIN)
    {

#if defined(ST25R3911)
        st25r3911Isr();
#elif defined(ST25R3916B)
        st25r3916Isr();
#endif
    }
}
#endif
