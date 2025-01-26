
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

    SenderDataSpec data;
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

                if (sender.initIndex >= EEPROM_NUM_SECTORS)
                {
                    if(!Eeprom_waiting())
                    {
                        // We can enter challenge mode if we are able to send a write command
                        if (Eeprom_writeNextSeqId())
                            sender.state = SENDER_STATE_CHALLENGE;
                    }
                    break;
                }
                else if (Eeprom_readSector(sender.initIndex))
                {
                    sender.initIndex++;
                }

                break;

            case SENDER_STATE_CHALLENGE:

                // Read the reader's header
                Eeprom_readReaderHeader();

                // Check to see if the challenge was responded
                if (eeprom.readerHeader.seqNum == (eeprom.senderHeader.seqNum + 1))
                {
                    sender.state = SENDER_STATE_IDLE;
                    break;
                }

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