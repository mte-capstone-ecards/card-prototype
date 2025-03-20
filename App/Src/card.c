#include "app.h"

#include <string.h>

#if BOARD(CARD, 1)
# include "m24lr_driver.h"
#elif BOARD(CARD, 2)
# include "m24lr.h"
#endif
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

#if FTR_GUI
#include "ugui.h"
#include "ugui_fonts.h"

static void GUI_pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    if (x > EINK_SCREEN_SIZE_H)
        return;
    if (y > EINK_SCREEN_SIZE_V)
        return;

    // Reflect vertically
    // x = EINK_SCREEN_SIZE_H - (x + 1);

    // Reflect horizontally
    // y = EINK_SCREEN_SIZE_V - (y + 1);

    if (color == C_BLACK)
        card.buf[y][x / 8] |= 1U << (7 - (x % 8));
    if (color == C_WHITE)
        card.buf[y][x / 8] &= ~(1U << (7 - (x % 8)));
}

static void GUI_flush(void)
{
    eink_powerUp();
    eink_fullUpdate(card.buf);
    eink_powerDown();

    // Does flushing need to clear the buffer?
    memset(card.buf, 0x00, sizeof(EPDBuf));
}

#define FONT_12 FONT_7X12
#define FONT_20 FONT_12X20

UG_GUI gui;
UG_DEVICE device = {
    .x_dim = EINK_SCREEN_SIZE_H,
    .y_dim = EINK_SCREEN_SIZE_V,
    .pset = GUI_pset,
    .flush = GUI_flush,
};

UG_WINDOW Card_window;

#define CARD_NUM_OBJECTS 6
UG_OBJECT Card_objects[CARD_NUM_OBJECTS];

UG_TEXTBOX Card_shape[5];
UG_TEXTBOX Card_num;

char *Card_shapeLabelMajor[] = {
    "*******",
    "+++++++",
    "$$$$$$$",
    "#######",
    "@@@@@@@",
};

char *Card_shapeLabelMinor[] = {
    "*     *",
    "+     +",
    "$     $",
    "#     #",
    "@     @",
};

char *Card_nums[] = {
    "A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", " ", "X", "O"
};

void Card_shapeDraw(UG_MESSAGE *msg)
{
    // Border
    UG_DrawFrame(UGUI_POS(2, 2, EINK_SCREEN_SIZE_H - 4, EINK_SCREEN_SIZE_V - 4), C_BLACK);
    UG_DrawFrame(UGUI_POS(8, 8, EINK_SCREEN_SIZE_H - 16, EINK_SCREEN_SIZE_V - 16), C_BLACK);
}

static void Card_loadCard()
{
    struct {
        uint8_t shape;
        uint8_t num;
        uint8_t dummy[2];
    } cardData;

    Eeprom_readData(0, 1, (uint32_t *) &cardData);

    UG_TextboxSetText(&Card_window, OBJ_ID_0, Card_shapeLabelMajor[cardData.shape]);
    UG_TextboxSetText(&Card_window, OBJ_ID_1, Card_shapeLabelMinor[cardData.shape]);
    UG_TextboxSetText(&Card_window, OBJ_ID_2, Card_shapeLabelMinor[cardData.shape]);
    UG_TextboxSetText(&Card_window, OBJ_ID_3, Card_shapeLabelMinor[cardData.shape]);
    UG_TextboxSetText(&Card_window, OBJ_ID_4, Card_shapeLabelMajor[cardData.shape]);
    UG_TextboxSetText(&Card_window, OBJ_ID_5, Card_nums[cardData.num]);

    UG_WindowShow(&Card_window);
    UG_Update();
}

static void Card_loadString()
{
    static char cardData[255];

    Eeprom_readData(0, eeprom.senderHeader.datalen, (uint32_t *) cardData);

    UG_TextboxSetText(&Card_window, OBJ_ID_0, cardData);
    UG_TextboxSetText(&Card_window, OBJ_ID_1, "");
    UG_TextboxSetText(&Card_window, OBJ_ID_2, "");
    UG_TextboxSetText(&Card_window, OBJ_ID_3, "");
    UG_TextboxSetText(&Card_window, OBJ_ID_4, "");
    UG_TextboxSetText(&Card_window, OBJ_ID_5, "");

    UG_WindowShow(&Card_window);
    UG_Update();
}


static void Card_constructHanabi(void)
{
    UG_Init(&gui, &device);

    UG_WindowCreate(&Card_window, Card_objects, CARD_NUM_OBJECTS, Card_shapeDraw);
    UG_WindowSetStyle(&Card_window, WND_STYLE_HIDE_TITLE);

    UG_TextboxCreate(&Card_window, &Card_shape[0], OBJ_ID_0, UGUI_POS(240/2 - ((16 * 7) / 2), 160 + 0 * 20, 16 * 7, 20));
    UG_TextboxSetFont(&Card_window, OBJ_ID_0, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_0, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_0, C_NONE);

    UG_TextboxCreate(&Card_window, &Card_shape[1], OBJ_ID_1, UGUI_POS(240/2 - ((16 * 7) / 2), 160 + 1 * 20, 16 * 7, 20));
    UG_TextboxSetFont(&Card_window, OBJ_ID_1, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_1, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_1, C_NONE);

    UG_TextboxCreate(&Card_window, &Card_shape[2], OBJ_ID_2, UGUI_POS(240/2 - ((16 * 7) / 2), 160 + 2 * 20, 16 * 7, 20));
    UG_TextboxSetFont(&Card_window, OBJ_ID_2, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_2, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_2, C_NONE);

    UG_TextboxCreate(&Card_window, &Card_shape[3], OBJ_ID_3, UGUI_POS(240/2 - ((16 * 7) / 2), 160 + 3 * 20, 16 * 7, 20));
    UG_TextboxSetFont(&Card_window, OBJ_ID_3, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_3, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_3, C_NONE);

    UG_TextboxCreate(&Card_window, &Card_shape[4], OBJ_ID_4, UGUI_POS(240/2 - ((16 * 7) / 2), 160 + 4 * 20, 16 * 7, 20));
    UG_TextboxSetFont(&Card_window, OBJ_ID_4, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_4, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_4, C_NONE);

    UG_TextboxCreate(&Card_window, &Card_num, OBJ_ID_5, UGUI_POS(40, 196, 160, 25));
    UG_TextboxSetFont(&Card_window, OBJ_ID_5, FONT_20);
    UG_TextboxSetText(&Card_window, OBJ_ID_5, "");
    UG_TextboxSetBackColor(&Card_window, OBJ_ID_5, C_NONE);

    // Enable if we want power on update
    memset(card.buf, 0x00, sizeof(EPDBuf));
    GUI_flush();
}
#endif

static void Card_initialize(void)
{
    eink_powerUp();
    memset(&card, 0U, sizeof(struct Card_S));

#if FTR_GUI
    Card_constructHanabi();
#endif

#if BOARD(CARD, 2)
    HAL_GPIO_WritePin(EINK_POWER_GPIO_Port, EINK_POWER_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M24LR_POWER_GPIO_Port, M24LR_POWER_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    M24LR_StatusTypeDef status = M24LR_i2c_IsDeviceReady(1);
	if (status != M24LR_OK)
	{
		card.state = CARD_STATE_ERROR;
	}
#else

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
#endif
}

// static void Card_loadPacket(void)
// {
//     // We have just received a packet, dispatch it to the eink
//     DataPacket *packet = (DataPacket *) &eeprom.data;

//     for (uint32_t i = 0; i < packet->header.dataLen; i++)
//     {
//         for (uint8_t bit = 0; bit < 32; bit++)
//         {
//             if (
//                 (packet->payload[i] & (1U << bit)) != 0U
//             )
//             {
//                 eink_setPixel(
//                     card.buf,
//                     card.pixelPointer % EINK_SCREEN_SIZE_H,
//                     card.pixelPointer / EINK_SCREEN_SIZE_H
//                 );
//             }

//             card.pixelPointer++;
//         }
//     }

// }

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
                            // Card_loadPacket();

                            Card_transmitHeader(RECEIVER_ACK);

                            break;

                        case SENDER_HANABI_INSTR:
                            // Handle a hanabi specific instruction
                            // Update card for Hanabi
                            Card_loadCard();
                            Card_transmitHeader(RECEIVER_NULL);

                            break;

                        case SENDER_STRING_INSTR:
                            Card_loadString();
                            Card_transmitHeader(RECEIVER_NULL);

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