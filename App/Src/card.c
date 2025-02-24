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
    CARD_STATE_IDLE,

    CARD_STATE_ERROR,           // Error, unrecoverable generally
} CardState;

struct Card_S {
    CardState state;

    bool open;
    uint8_t challengeValue;

    EPDBuf buf;
    uint32_t pixelPointer;

    volatile bool updated;
} card;

static void Card_initialize(void)
{
    eink_powerUp();
    memset(&card, 0U, sizeof(struct Card_S));

	M24lr_i2c_Drv.Init();

    M24LR_StatusTypeDef status = M24lr_i2c_Drv.IsReady(1);
	if (status != M24LR_OK)
	{
		card.state = CARD_STATE_ERROR;
	}

    M24lr_i2c_ExtDrv.SetRFWIP();
    M24lr_i2c_ExtDrv.SetEH();
    M24lr_i2c_ExtDrv.Enable_EH_mode();
    M24lr_i2c_ExtDrv.WriteEH_Cfg(M24LR_EH_Cfg_6MA);
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

void Card_transmitHeader(ReceiverInstruction instr)
{
    // As long as the function returns false (Fail) try again
    while (!Eeprom_writeNextHeader(instr))
    {
        HAL_Delay(50);
    }
}

void Card_busyCallback(void)
{
    card.updated = true;
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
                card.state = CARD_STATE_IDLE;
                card.updated = true;

                // We need to read our initial receiver header
                Eeprom_readReceiverHeader();

                break;

            case CARD_STATE_IDLE:
                // Wait until we receive a message
                if (!card.updated)
                    break;

                card.updated = false;
                Eeprom_readSenderHeader();

                if (eeprom.senderHeader.instruction == SENDER_CHALLENGE_INSTR)
                {
                    Card_transmitHeader(RECEIVER_NULL);
                    break;
                }

                if (Eeprom_partnerUpdated())
                {
                    switch (eeprom.senderHeader.instruction)
                    {
                        case SENDER_UPDATE_INSTR:
                            eink_fullUpdate(card.buf);
                            Card_transmitHeader(RECEIVER_UPDATED);
                            break;

                        case SENDER_DATA_INSTR:
                            // Read the whole eeprom
                            Eeprom_readAll();
                            Card_loadPacket();

                            Card_transmitHeader(RECEIVER_ACK);

                            break;

                        case SENDER_NULL_INSTR:
                        default:
                            Card_transmitHeader(RECEIVER_NULL);

                            break;
                    }
                }

                HAL_Delay(150);

                break;

            default:
                break;
        }
    }

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == M24LR_BUSY_Pin)
    {
        Card_busyCallback();
    }
}