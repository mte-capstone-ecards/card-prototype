
#include "app.h"
#include "ugui.h"
#include "ugui_fonts.h"
#include "eink.h"

EPDBuf buf;

static void GUI_pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    if (x > EINK_SCREEN_SIZE_H)
        return;
    if (y > EINK_SCREEN_SIZE_V)
        return;

    // if (color == C_BLACK)
    //     buf[y][x / 8] |= 1U << (x % 8);
    // if (color == C_WHITE)
    //     buf[y][x / 8] &= ~(1U << (x % 8));

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
    // memset(buf, 0x00, sizeof(EPDBuf));
}

UG_GUI gui;
UG_DEVICE device = {
    .x_dim = EINK_SCREEN_SIZE_H,
    .y_dim = EINK_SCREEN_SIZE_V,
    .pset = GUI_pset,
    .flush = GUI_flush,
};

#define MAX_OBJS 15
UG_WINDOW mainWindow;
UG_OBJECT mainWindowObjs[MAX_OBJS];
UG_TEXTBOX mainText;

static void windowHandler(UG_MESSAGE *msg)
{

}

void GUI_init()
{
    UG_Init(&gui, &device);
    UG_FillScreen(C_WHITE); // TODO: We could add a driver for fill screen (Memset)

    UG_WindowCreate(&mainWindow, mainWindowObjs, MAX_OBJS, &windowHandler);
    UG_WindowSetStyle(&mainWindow, WND_STYLE_HIDE_TITLE);

    UG_TextboxCreate(&mainWindow, &mainText, TXB_ID_0, UGUI_POS(40, 40, 100, 30));
    UG_TextboxSetFont(&mainWindow, TXB_ID_0, FONT_6X10);
    UG_TextboxSetText(&mainWindow, TXB_ID_0, "This is the text");

    UG_WindowShow(&mainWindow);

    // Doesn't work when the window is on top
    // UG_FillCircle(150, 120, 40, C_BLACK);

    UG_Update();
}
