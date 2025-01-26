
#include "sender.h"

#include <cmsis_os.h>
#include <string.h>

#include "eeprom.h"
#include "st25r.h"

#define SENDER_STATEMACHINE_PERIOD 5 // ms

typedef enum
{
    SENDER_STATE_DISCONNECTED = 0,
    SENDER_STATE_INIT,
    SENDER_STATE_CHALLENGE,
    SENDER_STATE_IDLE,
    SENDER_STATE_TRANSMITTING,
    SENDER_STATE_WAITING,
    SENDER_STATE_RETRANSMITTING,
    SENDER_STATE_CHECKING,
    SENDER_STATE_UPDATING,
    SENDER_STATE_ERROR,
} SenderState;

static struct {

    SenderState state;
    uint32_t initIndex;

} sender;

void Sender_task(void *args)
{
    (void) args;

    // Initialize sender struct
    memset(&sender, 0U, sizeof(sender));

    for (;;)
    {
        if ((sender.state != SENDER_STATE_DISCONNECTED) && !ST25R_connected())
        {
            sender.state = SENDER_STATE_ERROR;
            break;
        }

        switch (sender.state)
        {
            case SENDER_STATE_DISCONNECTED:
                if (ST25R_connected())
                {
                    sender.state = SENDER_STATE_INIT;
                    sender.initIndex = 0U;
                    break;
                }

                osDelay(10);
                break;

            case SENDER_STATE_INIT:

                if (sender.initIndex >= EEPROM_WORDS)
                {
                    extern osMessageQueueId_t nfcCommandQueueHandle;
                    if (osMessageQueueGetCount(nfcCommandQueueHandle) == 0)
                        sender.state = SENDER_STATE_CHALLENGE;
                    break;
                }

                static const uint8_t readNum = 1;
                if (Eeprom_readBytes(sender.initIndex, readNum, ((uint32_t *) &eeprom) + sender.initIndex))
                {
                    sender.initIndex += readNum;
                }

                break;

            case SENDER_STATE_CHALLENGE:

                osDelay(10);
                break;

            case SENDER_STATE_IDLE:

                break;

            case SENDER_STATE_TRANSMITTING:

                break;

            case SENDER_STATE_WAITING:

                break;

            case SENDER_STATE_RETRANSMITTING:

                break;

            case SENDER_STATE_CHECKING:

                break;

            case SENDER_STATE_UPDATING:

                break;

            case SENDER_STATE_ERROR:

                break;
        }
    }
}