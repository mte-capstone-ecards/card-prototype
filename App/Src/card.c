#include "app.h"

#include <string.h>

#include "m24lr_driver.h"
#include "eeprom.h"
#include "data_protocol.h"
#include "eink.h"

// M24lr_i2c_Drv
// M24lr_i2c_ExtDrv
#define CARD_TEST_LED_GPIO_Port GPIOA //idk?
#define CARD_TEST_LED_Pin 2 //idk this either

static const uint32_t max_voltage = 2;

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
}

static void lowPower_start(){
    //leds are for testing but currently have none? on card board 
    //HAL_GPIO_WritePin(CARD_TEST_LED_GPIO_Port, CARD_TEST_LED_Pin, 1);
    //HAL_Delay (3000); 
    HAL_SuspendTick(); //clock paused in low power mode
    //HAL_GPIO_WritePin(CARD_TEST_LED_GPIO_Port, CARD_TEST_LED_Pin, 0); 
    HAL_PWR_EnableSleepOnExit(); //sleep after everyinterrupt it is woken up by

    //HAL_PWR_EnterSLEEPMode (PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); //low poer mode ysed and return from interrupt

    //
    HAL_ResumeTick();
    //HAL_GPIO_WritePin(CARD_TEST_LED_GPIO_Port, CARD_TEST_LED_Pin, 1);

    //current is around 1.8 mA for stm32F103 (from resarch, ik this is a differnt stm32)
}

//in switch mode for state machine, if interrupt turns things on, use state machine to turn off if not enough change?
/*
HAL_ADC_ConvCplotCallback - this function will  be created by new stm32 ioc file -> need help :((
    uint32_t voltageDiff;
    if (voltageDiff >= max_voltage){
        HAL_ResumeTick();
        HAL_PWR_DisableSleepOnExit();
    }

*/

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
                card.state = CARD_STATE_IDLE;
                break;

            case CARD_STATE_IDLE:
                // Wait until we receive a message
                Eeprom_readSenderHeader();

                if (eeprom.senderHeader.instruction == SENDER_CHALLENGE_INSTR)
                {
                    Eeprom_writeNextHeader(RECEIVER_NULL);
                    break;
                }

                if (Eeprom_partnerUpdated())
                {
                    switch (eeprom.senderHeader.instruction)
                    {
                        case SENDER_UPDATE_INSTR:
                            eink_fullUpdate(card.buf);
                            Eeprom_writeNextHeader(RECEIVER_UPDATED);
                            break;

                        case SENDER_DATA_INSTR:
                            // Read the whole eeprom
                            Eeprom_readAll();
                            Card_loadPacket();

                            Eeprom_writeNextHeader(RECEIVER_ACK);

                            break;

                        case SENDER_NULL_INSTR:
                        default:
                            Eeprom_writeNextHeader(RECEIVER_NULL);

                            break;
                    }
                }

                HAL_Delay(90);

                break;

            default:
                break;
        }
    }

}