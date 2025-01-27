#include "app.h"

#include <string.h>

#include "m24lr_driver.h"
#include "eeprom.h"

// M24lr_i2c_Drv
// M24lr_i2c_ExtDrv

typedef enum
{
    CARD_STATE_POWERON = 0U,    // Initial state on power on
    CARD_STATE_CHALLENGE,       // Try to find a challenge
    CARD_STATE_IDLE,

    CARD_STATE_ERROR,           // Error, unrecoverable generally
} CardState;

struct Card_S {
    CardState state;

    uint8_t challengeValue;
} card;

static void Card_initialize()
{
    memset(&card, 0U, sizeof(struct Card_S));

	M24lr_i2c_Drv.Init();

    M24LR_StatusTypeDef status = M24lr_i2c_Drv.IsReady(1);
	if (status != M24LR_OK)
	{
		card.state = CARD_STATE_ERROR;
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
                break;

            default:
                break;
        }
    }

}