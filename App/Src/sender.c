
#include "sender.h"

#include <cmsis_os.h>
#include <string.h>

#include "eeprom.h"
#include "st25r.h"

#include "data_protocol.h"

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

    SenderDataSpec senderData;

    DataPacket packet;
} sender;

extern osMessageQueueId_t dataSenderQueueHandle;

static bool Sender_loadNextPacket(void)
{
    if (sender.senderData.numWords == 0)
        return false;

    // Clear existing packet data
    memset(&sender.packet, 0U, sizeof(DataPacket));

    // Determine transmission length
    uint16_t len = sender.senderData.numWords < DATA_PAYLOAD_SIZE ? sender.senderData.numWords : DATA_PAYLOAD_SIZE;
    sender.packet.header.dataLen = len;

    // Copy this amount of data into the payload
    memcpy(sender.packet.payload, sender.senderData.data + sender.senderData.startBit, 4 * len);

    // TODO: Add the CRC

    // Advance the start bit and subtract the length
    sender.senderData.startBit += len;
    sender.senderData.numWords -= len;

    return true;
}

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
                Eeprom_readReceiverHeader();

                // Check to see if the challenge was responded
                if (eeprom.receiverHeader.seqNum == (eeprom.senderHeader.seqNum + 1))
                {
                    sender.state = SENDER_STATE_IDLE;
                    break;
                }

                break;

            case SENDER_STATE_IDLE:

                osStatus_t status = osMessageQueueGet(dataSenderQueueHandle, &sender.senderData, NULL, 10);

                if (status == osOK)
                {
                    // We have received new data, we need to move into transmitting
                    sender.state = SENDER_STATE_TRANSMITTING;
                    break;
                }

                break;

            case SENDER_STATE_TRANSMITTING:
                if (Sender_loadNextPacket())
                {
                    Eeprom_writeData(0, (uint32_t *) &sender.packet, DATA_HEADER_SIZE + sender.packet.header.dataLen);

                    sender.state = SENDER_STATE_WAITING; // Wait for the response
                    break;
                }
                else
                {
                    sender.state = SENDER_STATE_CHECKING; // No data to transmit, check the transmission is correct
                    break;
                }

                break;

            case SENDER_STATE_WAITING:

                // Read the readers header, when we get a new reader message
                // Check if its ACK -> go to sender state transmitting
                // If its a NACK -> go to sender re-transmitting

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