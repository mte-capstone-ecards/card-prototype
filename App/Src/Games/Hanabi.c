#include "Games/Hanabi.h"

#include "gui.h"
#include "ugui.h"
#include "ugui_fonts.h"

#define FONT_12 FONT_7X12
#define FONT_20 FONT_12X20

UG_WINDOW Hanabi_window;

UG_TEXTBOX Hanabi_playerTurn;
UG_TEXTBOX Hanabi_cardIndicator[5][2];

UG_TEXTBOX Hanabi_lives[2];
UG_TEXTBOX Hanabi_cardMode[2];
UG_TEXTBOX Hanabi_clues[2];

UG_BUTTON Hanabi_exit;
UG_BUTTON Hanabi_modeSwitch;
UG_BUTTON Hanabi_giveClue;

typedef enum
{
    OBJ_PLAYER_TURN,
    OBJ_CARD0_NUM,
    OBJ_CARD0_SHAPE,
    OBJ_CARD1_NUM,
    OBJ_CARD1_SHAPE,
    OBJ_CARD2_NUM,
    OBJ_CARD2_SHAPE,
    OBJ_CARD3_NUM,
    OBJ_CARD3_SHAPE,
    OBJ_CARD4_NUM,
    OBJ_CARD4_SHAPE,
    OBJ_LIFE_LABEL,
    OBJ_LIFE_NUM,
    OBJ_MODE_LABEL,
    OBJ_MODE_CURRENT,
    OBJ_CLUE_LABEL,
    OBJ_CLUE_NUM,

    OBJ_EXIT,
    OBJ_MODE_SWITCH,
    OBJ_GIVE_CLUE,

    OBJ_COUNT
} Hanabi_objIds;
UG_OBJECT Hanabi_objs[OBJ_COUNT];

typedef enum
{
    SHAPE_STAR,
    SHAPE_PLUS,
    SHAPE_DOLLAR,
    SHAPE_HASHTAG,
    SHAPE_AT,

    SHAPE_COUNT,
} Shapes;

char *Hanabi_shapes[SHAPE_COUNT] = {
    [SHAPE_STAR]    = "*",
    [SHAPE_PLUS]    = "+",
    [SHAPE_DOLLAR]  = "$",
    [SHAPE_HASHTAG] = "#",
    [SHAPE_AT]      = "@",
};

#define MAX_STACK 5
char *Hanabi_nums[1 + MAX_STACK] = {
    "0", "1", "2", "3", "4", "5"
};

void Hanabi_shapeDraw(UG_MESSAGE* msg)
{
    UG_DrawLine(UGUI_POS(5, 200, 405, 0), C_BLACK);

    UG_DrawFrame(UGUI_POS(5, 35, 75, 120), C_BLACK);
    UG_DrawFrame(UGUI_POS(88, 35, 75, 120), C_BLACK);
    UG_DrawFrame(UGUI_POS(170, 35, 75, 120), C_BLACK);
    UG_DrawFrame(UGUI_POS(253, 35, 75, 120), C_BLACK);
    UG_DrawFrame(UGUI_POS(335, 35, 75, 120), C_BLACK);

}

void Hanabi_constructMenu(void)
{
    UG_WindowCreate(&Hanabi_window, Hanabi_objs, OBJ_COUNT, Hanabi_shapeDraw);
    UG_WindowSetStyle(&Hanabi_window, WND_STYLE_HIDE_TITLE);

    UG_TextboxCreate(&Hanabi_window, &Hanabi_playerTurn, OBJ_PLAYER_TURN, UGUI_POS(65, 0, 285, 15));
    UG_TextboxSetFont(&Hanabi_window, OBJ_PLAYER_TURN, FONT_12);
    UG_TextboxSetText(&Hanabi_window, OBJ_PLAYER_TURN, "PLAYER 1 TURN");

    UG_ButtonCreate(&Hanabi_window, &Hanabi_exit, OBJ_EXIT, UGUI_POS(10, 220, 120, 16));
    UG_ButtonSetFont(&Hanabi_window, OBJ_EXIT, FONT_12);
    UG_ButtonSetText(&Hanabi_window, OBJ_EXIT, "EXIT");
    UG_ButtonSetAlignment(&Hanabi_window, OBJ_EXIT, ALIGN_TOP_CENTER);

    UG_ButtonCreate(&Hanabi_window, &Hanabi_modeSwitch, OBJ_MODE_SWITCH, UGUI_POS(155, 220, 120, 16));
    UG_ButtonSetFont(&Hanabi_window, OBJ_MODE_SWITCH, FONT_12);
    UG_ButtonSetText(&Hanabi_window, OBJ_MODE_SWITCH, "SWITCH MODE");
    UG_ButtonSetAlignment(&Hanabi_window, OBJ_MODE_SWITCH, ALIGN_TOP_CENTER);

    UG_ButtonCreate(&Hanabi_window, &Hanabi_giveClue, OBJ_GIVE_CLUE, UGUI_POS(290, 220, 120, 16));
    UG_ButtonSetFont(&Hanabi_window, OBJ_GIVE_CLUE, FONT_12);
    UG_ButtonSetText(&Hanabi_window, OBJ_GIVE_CLUE, "GIVE CLUE");
    UG_ButtonSetAlignment(&Hanabi_window, OBJ_GIVE_CLUE, ALIGN_TOP_CENTER);

    GUI_highlightButton(&Hanabi_window, OBJ_GIVE_CLUE);
    GUI_unhighlightButton(&Hanabi_window, OBJ_MODE_SWITCH);
    GUI_unhighlightButton(&Hanabi_window, OBJ_EXIT);

    for (uint8_t card = 0; card < 5; card++)
    {
        UG_TextboxCreate(&Hanabi_window, &Hanabi_cardIndicator[card][0], OBJ_CARD0_NUM + 2 * card, UGUI_POS(34 + card * 83, 70, 20, 25));
        UG_TextboxSetFont(&Hanabi_window, OBJ_CARD0_NUM + 2 * card, FONT_20);
        UG_TextboxSetText(&Hanabi_window, OBJ_CARD0_NUM + 2 * card, Hanabi_nums[card]);

        UG_TextboxCreate(&Hanabi_window, &Hanabi_cardIndicator[card][1], OBJ_CARD0_SHAPE + 2 * card, UGUI_POS(34 + card * 83, 120, 20, 25));
        UG_TextboxSetFont(&Hanabi_window, OBJ_CARD0_SHAPE + 2 * card, FONT_20);
        UG_TextboxSetText(&Hanabi_window, OBJ_CARD0_SHAPE + 2 * card, Hanabi_shapes[card]);
    }
}

void Hanabi_setMenu(void)
{
    // Game Init
}

void Hanabi_updateMenu(void)
{
    UG_WindowShow(&Hanabi_window);
}

void Hanabi_buttonCallback(ButtonHandle button, PressType type)
{

}


