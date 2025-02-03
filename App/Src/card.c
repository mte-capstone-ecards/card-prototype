#include "app.h"

#include <string.h>

#include "m24lr_driver.h"
#include "eeprom.h"
#include "data_protocol.h"
#include "eink.h"

// M24lr_i2c_Drv
// M24lr_i2c_ExtDrv

typedef enum
{
    CARD_STATE_POWERON = 0U,    // Initial state on power on
    CARD_STATE_CHALLENGE,       // Try to find a challenge
    CARD_STATE_IDLE,
    CARD_STATE_UPDATE,

    CARD_STATE_ERROR,           // Error, unrecoverable generally
} CardState;

struct Card_S {
    CardState state;

    uint8_t challengeValue;
    EPDBuf buf;
    uint32_t pixelPointer;
} card;

static void Card_initialize(void)
{
    memset(&card, 0U, sizeof(struct Card_S));

	M24lr_i2c_Drv.Init();

    M24LR_StatusTypeDef status = M24lr_i2c_Drv.IsReady(1);
	if (status != M24LR_OK)
	{
		card.state = CARD_STATE_ERROR;
	}
}

static void Card_loadPacket(void)
{
    // We have just received a packet, dispatch it to the eink
    DataPacket *packet = (DataPacket *) &eeprom.data;

    for (uint32_t i = 0; i < packet->header.dataLen; i++)
    {
        for (uint8_t bit = 0; bit < 32; bit++)
        {
            if (
                (packet->payload[i] & (1U << bit)) != 0U
            )
            {
                eink_setPixel(
                    card.buf,
                    card.pixelPointer % EINK_SCREEN_SIZE_H,
                    card.pixelPointer / EINK_SCREEN_SIZE_H
                );
            }

            card.pixelPointer++;
        }
    }

}

void card_main(void)
{
    Card_initialize();

    for (;;)
    {
        switch (card.state)
        {
            case CARD_STATE_POWERON:

                // Should we only switch when field is on? This should be a given with EH
                card.state = CARD_STATE_CHALLENGE;

                break;

            case CARD_STATE_CHALLENGE:
                // Read the next sender header
                Eeprom_readSenderHeader();

                // Check if our previous challenge was accepted
                if (eeprom.senderHeader.seqNum == card.challengeValue)
                {
                    card.state = CARD_STATE_IDLE;
                    break;
                }

                // We failed... write the expected answer to the latest read header
                Eeprom_writeNextSeqId();
                card.challengeValue = eeprom.receiverHeader.seqNum + 1;

                HAL_Delay(5000);

                break;

            case CARD_STATE_IDLE:
                // TODO: Idle state is currently idle and receiving, maybe want to seperate

                // Wait until we receive a message
                Eeprom_readSenderHeader();

                if (!Eeprom_partnerStale())
                {

                    if (eeprom.senderHeader.update)
                    {
                        card.state = CARD_STATE_UPDATE;
                        break;
                    }

                    // Read the whole eeprom
                    Eeprom_readAll();

                    Card_loadPacket();

                    Eeprom_writeNextSeqId();
                }

                break;

            case CARD_STATE_UPDATE:

                eink_fullUpdate(card.buf);

                eeprom.receiverHeader.updated = 1;
                Eeprom_writeNextSeqId();

                card.state = CARD_STATE_CHALLENGE;

                break;

            default:
                break;
        }
    }

}