
#include "app.h"
#include "ugui.h"
#include "ugui_fonts.h"
#include "eink.h"
#include "button.h"
#include <string.h>

EPDBuf buf;

/***************************************
            Eink driver functions
****************************************/
static void GUI_pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    if (x > EINK_SCREEN_SIZE_V)
        return;
    if (y > EINK_SCREEN_SIZE_H)
        return;

    // Reflect vertically
    x = EINK_SCREEN_SIZE_V - (x + 1);

    // Reflect horizontally
    y = EINK_SCREEN_SIZE_H - (y + 1);

    if (color == C_BLACK)
        buf[x][y / 8] |= 1U << (7 - (y % 8));
    if (color == C_WHITE)
        buf[x][y / 8] &= ~(1U << (7 - (y % 8)));
}

static void GUI_flush(void)
{
    eink_powerUp();
    eink_fullUpdate(buf);
    eink_powerDown();

    // Does flushing need to clear the buffer?
    memset(buf, 0x00, sizeof(EPDBuf));
}


/***************************************
            UGui
****************************************/
UG_GUI gui;
UG_DEVICE device = {
    .x_dim = EINK_SCREEN_SIZE_V,
    .y_dim = EINK_SCREEN_SIZE_H,
    .pset = GUI_pset,
    .flush = GUI_flush,
};

#define MAX_OBJS 15
UG_WINDOW mainWindow;
UG_OBJECT mainWindowObjs[MAX_OBJS];
UG_TEXTBOX mainText;
UG_BUTTON mainButtons[3];

typedef enum
{
    MENU_MAIN,
    MENU_COUNT
} MenuScreen;

typedef struct
{
    uint8_t maxButton;
} Menu;

Menu menus[MENU_COUNT] = {
    [MENU_MAIN] = {
        .maxButton = 3,
    },
};

struct {
    MenuScreen currentMenu;
    uint8_t selectedButton;
} menuGui;


static void windowHandler(UG_MESSAGE *msg)
{

}

static void GUI_highlightButtons()
{
    // for (uint8_t button = 0; button < menus[menuGui.currentMenu].maxButton; button++)
    // {
    //     UG_ButtonSetBackColor(&mainWindow, button, C_WHITE);
    //     UG_ButtonSetForeColor(&mainWindow, button, C_BLACK);
    // }
    UG_ButtonSetBackColor(&mainWindow, BTN_ID_0, C_WHITE);
    UG_ButtonSetForeColor(&mainWindow, BTN_ID_0, C_BLACK);
    UG_ButtonSetBackColor(&mainWindow, BTN_ID_1, C_WHITE);
    UG_ButtonSetForeColor(&mainWindow, BTN_ID_1, C_BLACK);
    UG_ButtonSetBackColor(&mainWindow, BTN_ID_2, C_WHITE);
    UG_ButtonSetForeColor(&mainWindow, BTN_ID_2, C_BLACK);

    UG_ButtonSetBackColor(&mainWindow, menuGui.selectedButton, C_BLACK);
    UG_ButtonSetForeColor(&mainWindow, menuGui.selectedButton, C_WHITE);
}

void GUI_buttonHandler(ButtonHandle button)
{
    // switch (menuGui.currentMenu)
    // {

    // }
}

static void showMenu(MenuScreen menu)
{
    menuGui.currentMenu = menu;

    switch (menu)
    {
        case MENU_MAIN:
            UG_WindowCreate(&mainWindow, mainWindowObjs, MAX_OBJS, &windowHandler);
            UG_WindowSetStyle(&mainWindow, WND_STYLE_HIDE_TITLE);

            UG_TextboxCreate(&mainWindow, &mainText, TXB_ID_5, UGUI_POS(device.x_dim / 2 - 60, 0, 120, 10));
            UG_TextboxSetFont(&mainWindow, TXB_ID_5, FONT_6X10);
            UG_TextboxSetText(&mainWindow, TXB_ID_5, "Electronic Card Deck");

            UG_ButtonCreate(&mainWindow, &mainButtons[0], BTN_ID_0, UGUI_POS(25, device.y_dim - 50, 100, 15));
            UG_ButtonSetFont(&mainWindow, BTN_ID_0, FONT_6X10);
            UG_ButtonSetText(&mainWindow, BTN_ID_0, "Button 0");
            UG_ButtonSetAlignment(&mainWindow, BTN_ID_0, ALIGN_TOP_CENTER);

            UG_ButtonCreate(&mainWindow, &mainButtons[1], BTN_ID_1, UGUI_POS(150, device.y_dim - 50, 100, 15));
            UG_ButtonSetFont(&mainWindow, BTN_ID_1, FONT_6X10);
            UG_ButtonSetText(&mainWindow, BTN_ID_1, "Button 1");
            UG_ButtonSetAlignment(&mainWindow, BTN_ID_1, ALIGN_TOP_CENTER);

            UG_ButtonCreate(&mainWindow, &mainButtons[2], BTN_ID_2, UGUI_POS(275, device.y_dim - 50, 100, 15));
            UG_ButtonSetFont(&mainWindow, BTN_ID_2, FONT_6X10);
            UG_ButtonSetText(&mainWindow, BTN_ID_2, "Button 2");
            UG_ButtonSetAlignment(&mainWindow, BTN_ID_2, ALIGN_TOP_CENTER);

            menuGui.selectedButton = BTN_ID_0;
            GUI_highlightButtons();
            UG_WindowShow(&mainWindow);

            break;

        default:
            break;
    }

    UG_Update();
}

#include <cmsis_os.h>

void GUI_init()
{
    UG_Init(&gui, &device);
    UG_FillScreen(C_WHITE); // TODO: We could add a driver for fill screen (Memset)

    showMenu(MENU_MAIN);
}
