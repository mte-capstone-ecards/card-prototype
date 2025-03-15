
#include <gpio.h>
#include <string.h>

#include "eink.h"
#include "ugui.h"
#include "ugui_fonts.h"

#include "m24lr.h"
#include "eeprom.h"

EPDBuf buf;

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
        buf[y][x / 8] |= 1U << (7 - (x % 8));
    if (color == C_WHITE)
        buf[y][x / 8] &= ~(1U << (7 - (x % 8)));
}

static void GUI_flush(void)
{
    eink_powerUp();
    eink_fullUpdate(buf);
    eink_powerDown();

    // Does flushing need to clear the buffer?
    memset(buf, 0x00, sizeof(EPDBuf));
}

#define FONT_20 FONT_12X20

UG_GUI gui;
UG_DEVICE device = {
    .x_dim = EINK_SCREEN_SIZE_H,
    .y_dim = EINK_SCREEN_SIZE_V,
    .pset = GUI_pset,
    .flush = GUI_flush,
};

typedef enum
{
    TEST_M24LR_COMM,
    TEST_UUID,
    TEST_NFC_IRQ,

    TEST_COUNT,
} Test;

typedef enum
{
    TEST_NOT_STARTED,
    TEST_INPROGRESS,

    // Test done statuses
    TEST_FAIL,
    TEST_SUCCEED,

    TEST_COMPLETE = TEST_FAIL,
    TEST_INCOMPLETE = TEST_INPROGRESS,
} TestState;

TestState tests[TEST_COUNT] = { 0U };

char *testDescs[TEST_COUNT] = {
    [TEST_M24LR_COMM] = "Comms with M24LR: ",
    [TEST_UUID] = "UUID: ",
    [TEST_NFC_IRQ] = "Detect NFC Write: ",
};

char *testResults[] = {
    [TEST_NOT_STARTED] = "Not Started",
    [TEST_FAIL] = "Fail",
    [TEST_SUCCEED] = "Suceed"
};

char *hex[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
};


#define TEST_NOTDONE(t) (tests[t] <= TEST_INCOMPLETE)
#define TEST_COMPLETE(t) (tests[t] >= TEST_COMPLETE)
#define TEST_PASS(t) (tests[t] == TEST_SUCCEED)

void card_executeTests(void)
{
    static uint8_t UUIDretries = 5;

    for (Test test = 0; test < TEST_COUNT; test++)
    {
        switch (test)
        {
            case TEST_M24LR_COMM:

                if (TEST_NOTDONE(TEST_M24LR_COMM))
                {
                    M24LR_StatusTypeDef status = M24LR_i2c_IsDeviceReady(1);
                    tests[TEST_M24LR_COMM] = status == M24LR_OK ? TEST_SUCCEED : TEST_FAIL;
                }

                break;

            case TEST_UUID:
                if (TEST_NOTDONE(TEST_UUID))
                {
                    if (TEST_PASS(TEST_M24LR_COMM))
                    {
                        if (Eeprom_readUUId())
                        {
                            tests[test] = TEST_SUCCEED;
                        }
                        else if (UUIDretries == 0)
                        {
                            tests[test] = TEST_FAIL;
                            break;
                        }
                        UUIDretries--;
                    }
                }

                break;

            case TEST_NFC_IRQ:

                break;

            default:
                break;
        }
    }
}


UG_WINDOW Card_window;

UG_TEXTBOX m24CommLabel;
UG_TEXTBOX m24CommResults;

UG_TEXTBOX uuidLabel;
UG_TEXTBOX uuidResults[8];

#define Card_objectCount 11
UG_OBJECT Card_objs[Card_objectCount];

void Card_shapeDraw(UG_MESSAGE *msg)
{
    (void) msg;


}

void card_displayTests(void)
{
    static bool updated = false;

    if (TEST_COMPLETE(TEST_M24LR_COMM) && TEST_COMPLETE(TEST_UUID))
    {
        if (!updated)
        {
            UG_WindowCreate(&Card_window, Card_objs, Card_objectCount, Card_shapeDraw);
            UG_WindowSetStyle(&Card_window, WND_STYLE_HIDE_TITLE);

            UG_TextboxCreate(&Card_window, &m24CommLabel, OBJ_ID_0, UGUI_POS(20, 20, 100, 25));
            UG_TextboxSetFont(&Card_window, OBJ_ID_0, FONT_20);
            UG_TextboxSetText(&Card_window, OBJ_ID_0, testDescs[TEST_M24LR_COMM]);

            UG_TextboxCreate(&Card_window, &m24CommResults, OBJ_ID_1, UGUI_POS(120, 20, 100, 25));
            UG_TextboxSetFont(&Card_window, OBJ_ID_1, FONT_20);
            UG_TextboxSetText(&Card_window, OBJ_ID_1, testResults[tests[TEST_M24LR_COMM]]);

            UG_TextboxCreate(&Card_window, &m24CommLabel, OBJ_ID_2, UGUI_POS(20, 60, 80, 25));
            UG_TextboxSetFont(&Card_window, OBJ_ID_2, FONT_20);
            UG_TextboxSetText(&Card_window, OBJ_ID_2, testDescs[TEST_UUID]);

            for (uint8_t i = 0; i < 8; i++)
            {
                UG_TextboxCreate(&Card_window, &uuidResults[i], OBJ_ID_3 + i, UGUI_POS(80, 60, 20, 25));
                UG_TextboxSetFont(&Card_window, OBJ_ID_3 + i, FONT_20);
                UG_TextboxSetText(&Card_window, OBJ_ID_3 + i, hex[0xF & (eeprom.UUID >> (i * 4))]);
            }

            UG_WindowShow(&Card_window);
            UG_Update();

            updated = true;
        }
    }
}

void card_bist(void)
{
    UG_Init(&gui, &device);
    UG_Update();

    // Enter the end state: Blink the LEDs
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);

    while(1)
    {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
        HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);

        card_executeTests();
        card_displayTests();
    }
}
