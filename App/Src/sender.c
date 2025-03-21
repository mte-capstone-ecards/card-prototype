
#include "sender.h"

#if FTR_DATASENDER

#include <cmsis_os.h>
#include <string.h>

#include "eeprom.h"
#include "st25r.h"

#include "data_protocol.h"
#include "games.h"
#include "gui.h"

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
    SENDER_STATE_COOLDOWN,
} SenderState;

static struct {

    SenderState state;
    uint32_t initIndex;

    SenderDataSpec senderData;

    DataPacket packet;
} sender;

extern osMessageQueueId_t dataSenderQueueHandle;

// static uint16_t Sender_loadNextPacket(void)
// {
//     if (sender.senderData.numWords == 0)
//         return false;

//     // Clear existing packet data
//     memset(&sender.packet, 0U, sizeof(DataPacket));

//     // Determine transmission length
//     uint16_t len = sender.senderData.numWords < DATA_PAYLOAD_SIZE ? sender.senderData.numWords : DATA_PAYLOAD_SIZE;
//     sender.packet.header.dataLen = len;

//     // Copy this amount of data into the payload
//     memcpy(sender.packet.payload, sender.senderData.data + sender.senderData.startBit, 4 * len);

//     // TODO: Add the CRC

//     return len;
// }

void Sender_writeData(void)
{
    uint32_t msg;

    switch (sender.senderData.instr)
    {
        case SENDER_HANABI_INSTR:
        case SENDER_CARD_INSTR:
            msg = ((sender.senderData.card.suit << 0U) | (sender.senderData.card.num << 8U)) & 0x0000FFFF;
            Eeprom_writeData(0, &msg, 1);

            break;
        case SENDER_STRING_INSTR:
            msg = (
                ((0xFF & sender.senderData.string.indices[0]) << 0U) |
                ((0xFF & sender.senderData.string.indices[1]) << 8U) |
                ((0xFF & sender.senderData.string.indices[2]) << 16U)  |
                ((0xFF & sender.senderData.string.indices[3]) << 24U)
            );

            Eeprom_writeData(0, &msg, 1);
            Eeprom_writeData(1, (uint32_t *) sender.senderData.string.str, sender.senderData.string.len);

            break;

        default:
            break;
    }
}

void Sender_task(void *args)
{
    osStatus status = osOK;
    (void) args;

    // Initialize sender struct
    memset(&sender, 0U, sizeof(sender));

    for (;;)
    {
        if ((sender.state != SENDER_STATE_DISCONNECTED) && !ST25R_connected())
        {
            sender.state = SENDER_STATE_ERROR;
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

                break;

            case SENDER_STATE_INIT:

                if (sender.initIndex >= EEPROM_NUM_SECTORS)
                {
                    if(!Eeprom_waiting())
                    {
                        // We can enter challenge mode if we are able to send a write command
                        if (Eeprom_writeNextHeader(SENDER_CHALLENGE_INSTR))
                        {
                            sender.state = SENDER_STATE_CHALLENGE;
                        }
                    }

                    break;
                }
                else if (Eeprom_readSector(sender.initIndex))
                {
                    sender.initIndex++;
                }

                break;

            case SENDER_STATE_CHALLENGE:

                // Read the receiver's header
                Eeprom_readReceiverHeader();

                // Check to see if the challenge was responded
                if (Eeprom_partnerUpdated())
                {
                    // We connected! Trigger the card callback
                    if (GUI_cardTap(eeprom.UUID))
                    {
                        sender.state = SENDER_STATE_IDLE;
                    }
                    break;
                }
                Eeprom_writeNextHeader(SENDER_NULL_INSTR);
                Eeprom_writeNextHeader(SENDER_CHALLENGE_INSTR);
                break;

            case SENDER_STATE_IDLE:
#if 1
                // status = osMessageQueueGet(dataSenderQueueHandle, &sender.senderData, NULL, 10);
                sender.senderData = Game_sendCard(eeprom.UUID);

                Sender_writeData();
                Eeprom_writeNextHeader(SENDER_HANABI_INSTR);

                sender.initIndex = 0;
                GUI_updateCurrentMenu();
                sender.state = SENDER_STATE_WAITING;
                break;
#else
                status = osMessageQueueGet(dataSenderQueueHandle, &sender.senderData, NULL, 10);

                if (status == osOK)
                {
                    // We have received new data, we need to move into transmitting
                    sender.state = SENDER_STATE_TRANSMITTING;
                    break;
                }
#endif
                break;

            // case SENDER_STATE_TRANSMITTING:
            //     if (Sender_loadNextPacket())
            //     {
            //         if (Eeprom_writeData(0, (uint32_t *) &sender.packet, DATA_HEADER_SIZE + sender.packet.header.dataLen))
            //         {
            //         }
            //         else
            //         {
            //             // No room in queue, try again
            //             break;
            //         }

            //         while (!Eeprom_writeNextHeader(SENDER_DATA_INSTR))
            //             osThreadYield();

            //         sender.state = SENDER_STATE_WAITING; // Wait for the response
            //         break;
            //     }
            //     else
            //     {
            //         sender.state = SENDER_STATE_CHECKING; // No data to transmit, check the transmission is correct
            //         break;
            //     }

            //     break;

            case SENDER_STATE_WAITING:
                osDelay(50);
                break;

            //     // Read the receiver's header
            //     Eeprom_readReceiverHeader();

            //     if (Eeprom_partnerUpdated())
            //     {
            //         // Read the receivers header, when we get a new receiver message
            //         // Check if its ACK -> go to sender state transmitting
            //         // If its a NACK -> go to sender re-transmitting

            //         if (eeprom.receiverHeader.instruction == RECEIVER_ACK)
            //         {
            //             // Advance the start bit and subtract the length
            //             sender.senderData.startBit += sender.packet.header.dataLen;
            //             sender.senderData.numWords -= sender.packet.header.dataLen;

            //             sender.state = SENDER_STATE_TRANSMITTING;
            //         }
            //         else
            //         {
            //             sender.state = SENDER_STATE_RETRANSMITTING;
            //         }

            //         break;
            //     }

            //     break;

            // case SENDER_STATE_RETRANSMITTING:

            //     break;

            // case SENDER_STATE_CHECKING:

            //     // Not implemented, go straight to updating
            //     sender.state = SENDER_STATE_UPDATING;

            //     break;

            // case SENDER_STATE_UPDATING:

            //     // Send a command requesting the device to update the display
            //     if (!Eeprom_writeNextHeader(SENDER_UPDATE_INSTR))
            //     {
            //         break;
            //     }

            //     // Read the receiver's header
            //     while(eeprom.receiverHeader.instruction != RECEIVER_UPDATED)
            //     {
            //         Eeprom_readReceiverHeader();
            //     }

            //     // It updated! We can send next ID and go back to idle.
            //     if (!Eeprom_writeNextHeader(SENDER_NULL_INSTR))
            //     {
            //         sender.state = SENDER_STATE_ERROR;
            //         break;
            //     }

            //     sender.state = SENDER_STATE_IDLE;
            //     break;

            //     break;

            case SENDER_STATE_ERROR:
            default:

                // Reset sender
                memset(&sender, 0U, sizeof(sender));

                osDelay(100);

                break;

            case SENDER_STATE_COOLDOWN:
                sender.initIndex++;
                if (sender.initIndex >= 32)
                {
                    memset(&sender, 0U, sizeof(sender));
                    break;
                }

                break;
        }

        Watchdog_tickle(THREAD_SENDER);
        osDelay(THREAD_SENDER_PERIOD);
    }
}

#endif
