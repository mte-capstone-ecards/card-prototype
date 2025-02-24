
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

static void windowHandler(UG_MESSAGE *msg)
{

}

void GUI_init()
{
    UG_Init(&gui, &device);
    UG_FillScreen(C_BLACK);

    UG_WindowCreate(&mainWindow, mainWindowObjs, MAX_OBJS, &windowHandler);
    UG_WindowSetTitleTextFont (&mainWindow, FONT_6X10);
    UG_WindowSetTitleText(&mainWindow, "App Title");

    UG_WindowShow(&mainWindow);

    UG_Update();
}
