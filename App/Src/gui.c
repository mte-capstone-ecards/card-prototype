#include "gui.h"

#include "app.h"
#include "ugui.h"
#include "ugui_fonts.h"
#include "eink.h"
#include "button.h"
#include "games.h"
#include <string.h>
#include <cmsis_os.h>

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
    // x = EINK_SCREEN_SIZE_V - (x + 1);

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

typedef enum
{
    MENU_MAIN,
    MENU_GAME_SELECT,
    MENU_PLAYER_SELECT,
    MENU_CARD_LOAD,
    MENU_GAME,
    MENU_ABOUT,

    MENU_COUNT
} MenuScreen;

MenuScreen GUI_currentMenu = MENU_MAIN;
uint8_t GUI_selectedButton = 0;

/*
    Main Menu Objects
*/
UG_WINDOW MainMenu_window;

UG_TEXTBOX MainMenu_title;
UG_BUTTON MainMenu_gameSelect;
UG_BUTTON MainMenu_about;

#define MainMenu_objectCount 3
UG_OBJECT MainMenu_objs[MainMenu_objectCount];

/*
    Game Select Objects
*/
UG_WINDOW GameSelect_window;

UG_TEXTBOX GameSelect_title;
UG_TEXTBOX GameSelect_leftArrow;
UG_TEXTBOX GameSelect_game;
UG_TEXTBOX GameSelect_rightArrow;
UG_BUTTON GameSelect_back;

#define GameSelect_objectCount 7
UG_OBJECT GameSelect_objs[GameSelect_objectCount];

uint8_t GameSelect_selectedGame;

/*
    Player Select
*/
UG_WINDOW PlayerSelect_window;

UG_TEXTBOX GUI_selectedGameTitle;
UG_TEXTBOX PlayerSelect_2PlayerPair[2];
UG_TEXTBOX PlayerSelect_3PlayerPair[2];
UG_TEXTBOX PlayerSelect_4PlayerPair[2];
UG_BUTTON PlayerSelect_back;

#define PlayerSelect_objectCount 8
UG_OBJECT PlayerSelect_objs[PlayerSelect_objectCount];

static uint8_t PlayerSelect_players = 2;

uint8_t GUI_selectedGame;

/*
    Card Load
*/
UG_WINDOW CardLoad_window;

UG_TEXTBOX CardLoad_gameTitle;
UG_TEXTBOX CardLoad_subtitle;
UG_TEXTBOX CardLoad_instruction;
UG_PROGRESS CardLoad_progress;
UG_BUTTON CardLoad_back;

#define CardLoad_objectCount 8
UG_OBJECT CardLoad_objs[CardLoad_objectCount];

char *CardLoad_strings[4][5] = {
    {
        "Player 1 - Tap Card 1",
        "Player 1 - Tap Card 2",
        "Player 1 - Tap Card 3",
        "Player 1 - Tap Card 4",
        "Player 1 - Tap Card 5",
    },
    {
        "Player 2 - Tap Card 1",
        "Player 2 - Tap Card 2",
        "Player 2 - Tap Card 3",
        "Player 2 - Tap Card 4",
        "Player 2 - Tap Card 5",
    },
    {
        "Player 3 - Tap Card 1",
        "Player 3 - Tap Card 2",
        "Player 3 - Tap Card 3",
        "Player 3 - Tap Card 4",
        "Player 3 - Tap Card 5",
    },
    {
        "Player 4 - Tap Card 1",
        "Player 4 - Tap Card 2",
        "Player 4 - Tap Card 3",
        "Player 4 - Tap Card 4",
        "Player 4 - Tap Card 5",
    },
};

/*
    About Objects
*/

/*

*/

static void MainMenu_shapeDraw(UG_MESSAGE *msg)
{
    UG_DrawLine(UGUI_POS(70, 100, 275, 0), C_BLACK);
}

static void GameSelect_shapeDraw(UG_MESSAGE *msg)
{
    UG_DrawLine(UGUI_POS(5, 35, 405, 0), C_BLACK);

    if (GUI_selectedButton == 0)
    {
        UG_FillFrame(UGUI_POS(65, 100, 285, 45), C_BLACK);

        UG_TextboxSetForeColor(&GameSelect_window, OBJ_ID_2, C_WHITE);
        UG_TextboxSetBackColor(&GameSelect_window, OBJ_ID_2, C_NONE);
        UG_OBJECT *obj = (UG_OBJECT*) &(GameSelect_window.objlst[OBJ_ID_2]);

        obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
        obj->update(&GameSelect_window, obj);
        UG_DrawFrame(UGUI_POS(65 + 2, 100 + 2, 285 - 4, 45 - 4), C_WHITE);
    }
    else
    {
        UG_TextboxSetForeColor(&GameSelect_window, OBJ_ID_2, C_BLACK);
        UG_TextboxSetBackColor(&GameSelect_window, OBJ_ID_2, C_NONE);
        UG_OBJECT *obj = (UG_OBJECT*) &(GameSelect_window.objlst[OBJ_ID_2]);

        obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
        obj->update(&GameSelect_window, obj);
    }
}

static void PlayerSelect_selectPlayerCount(uint8_t player)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        UG_TextboxSetForeColor(&PlayerSelect_window, 1 + i, C_BLACK);
        UG_TextboxSetBackColor(&PlayerSelect_window, 1 + i, C_NONE);

    }

    if (2 <= player && player <= 4)
    {
        const uint8_t selected = player - 2;
        UG_TextboxSetForeColor(&PlayerSelect_window, OBJ_ID_1 + 2 * selected, C_WHITE);
        UG_TextboxSetBackColor(&PlayerSelect_window, OBJ_ID_1 + 2 * selected, C_NONE);
        UG_TextboxSetForeColor(&PlayerSelect_window, OBJ_ID_2 + 2 * selected, C_WHITE);
        UG_TextboxSetBackColor(&PlayerSelect_window, OBJ_ID_2 + 2 * selected, C_NONE);
    }
}

static void PlayerSelect_shapeDraw(UG_MESSAGE *msg)
{
    UG_DrawLine(UGUI_POS(5, 35, 405, 0), C_BLACK);

    if (2 <= PlayerSelect_players && PlayerSelect_players <= 4)
    {
        uint8_t selected = PlayerSelect_players - 2;

        UG_FillFrame(UGUI_POS(135, 50 + 50 * selected, 145, 50), C_BLACK);

        UG_OBJECT *obj1 = (UG_OBJECT*) &(PlayerSelect_window.objlst[OBJ_ID_1 + 2 * selected]);
        UG_OBJECT *obj2 = (UG_OBJECT*) &(PlayerSelect_window.objlst[OBJ_ID_2 + 2 * selected]);

        obj1->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
        obj1->update(&PlayerSelect_window, obj1);
        obj2->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
        obj2->update(&PlayerSelect_window, obj2);

        UG_DrawFrame(UGUI_POS(135 + 2, 50 + 50 * selected + 2, 145 - 4, 50 - 4), C_WHITE);
    }

}

static void CardLoad_shapeDraw(UG_MESSAGE *msg)
{
    UG_DrawLine(UGUI_POS(5, 35, 405, 0), C_BLACK);

    // UG_DrawFrame(UGUI_POS(65 + 2, 100 + 2, 285 - 4, 45 - 4), C_WHITE);
}

void GUI_highlightButton(UG_WINDOW *wnd, UG_U8 id)
{
    UG_ButtonSetForeColor(wnd, id, C_WHITE);
    UG_ButtonSetBackColor(wnd, id, C_BLACK);
}

void GUI_unhighlightButton(UG_WINDOW *wnd, UG_U8 id)
{
    UG_ButtonSetForeColor(wnd, id, C_BLACK);
    UG_ButtonSetBackColor(wnd, id, C_WHITE);
}

static void GUI_constructMenus()
{
    for (MenuScreen menu = 0; menu < MENU_COUNT; menu++)
    {
        switch (menu)
        {
            case MENU_MAIN:
                UG_WindowCreate(&MainMenu_window, MainMenu_objs, MainMenu_objectCount, MainMenu_shapeDraw);
                UG_WindowSetStyle(&MainMenu_window, WND_STYLE_HIDE_TITLE);

                UG_TextboxCreate(&MainMenu_window, &MainMenu_title, OBJ_ID_0, UGUI_POS(126, 46, 164, 25));
                UG_TextboxSetFont(&MainMenu_window, OBJ_ID_0, FONT_20);
                UG_TextboxSetText(&MainMenu_window, OBJ_ID_0, "GAME HUB");

                UG_ButtonCreate(&MainMenu_window, &MainMenu_gameSelect, OBJ_ID_1, UGUI_POS(40, 160, 130, 16));
                UG_ButtonSetFont(&MainMenu_window, OBJ_ID_1, FONT_12);
                UG_ButtonSetText(&MainMenu_window, OBJ_ID_1, "START");
                UG_ButtonSetAlignment(&MainMenu_window, OBJ_ID_1, ALIGN_TOP_CENTER);

                UG_ButtonCreate(&MainMenu_window, &MainMenu_about, OBJ_ID_2, UGUI_POS(240, 160, 130, 16));
                UG_ButtonSetFont(&MainMenu_window, OBJ_ID_2, FONT_12);
                UG_ButtonSetText(&MainMenu_window, OBJ_ID_2, "ABOUT");
                UG_ButtonSetAlignment(&MainMenu_window, OBJ_ID_2, ALIGN_TOP_CENTER);

                break;

            case MENU_GAME_SELECT:
                UG_WindowCreate(&GameSelect_window, GameSelect_objs, GameSelect_objectCount, GameSelect_shapeDraw);
                UG_WindowSetStyle(&GameSelect_window, WND_STYLE_HIDE_TITLE);

                UG_TextboxCreate(&GameSelect_window, &GameSelect_title, OBJ_ID_0, UGUI_POS(10, 0, 140, 25));
                UG_TextboxSetFont(&GameSelect_window, OBJ_ID_0, FONT_20);
                UG_TextboxSetText(&GameSelect_window, OBJ_ID_0, "GAME LIST");

                UG_TextboxCreate(&GameSelect_window, &GameSelect_leftArrow, OBJ_ID_1, UGUI_POS(35, 105, 20, 15));
                UG_TextboxSetFont(&GameSelect_window, OBJ_ID_1, FONT_20);
                UG_TextboxSetText(&GameSelect_window, OBJ_ID_1, "<");

                UG_TextboxCreate(&GameSelect_window, &GameSelect_game, OBJ_ID_2, UGUI_POS(65, 100, 285, 25));
                UG_TextboxSetFont(&GameSelect_window, OBJ_ID_2, FONT_20);
                UG_TextboxSetText(&GameSelect_window, OBJ_ID_2, "");

                UG_TextboxCreate(&GameSelect_window, &GameSelect_rightArrow, OBJ_ID_3, UGUI_POS(365, 105, 20, 15));
                UG_TextboxSetFont(&GameSelect_window, OBJ_ID_3, FONT_20);
                UG_TextboxSetText(&GameSelect_window, OBJ_ID_3, ">");

                UG_ButtonCreate(&GameSelect_window, &GameSelect_back, OBJ_ID_4, UGUI_POS(10, 200, 120, 16));
                UG_ButtonSetFont(&GameSelect_window, OBJ_ID_4, FONT_12);
                UG_ButtonSetText(&GameSelect_window, OBJ_ID_4, "<- BACK");
                UG_ButtonSetAlignment(&GameSelect_window, OBJ_ID_4, ALIGN_TOP_CENTER);

            case MENU_PLAYER_SELECT:
                UG_WindowCreate(&PlayerSelect_window, PlayerSelect_objs, PlayerSelect_objectCount, PlayerSelect_shapeDraw);
                UG_WindowSetStyle(&PlayerSelect_window, WND_STYLE_HIDE_TITLE);

                UG_TextboxCreate(&PlayerSelect_window, &GUI_selectedGameTitle, OBJ_ID_0, UGUI_POS(65, 0, 285, 25));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_0, FONT_20);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_0, "");

                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_2PlayerPair[0], OBJ_ID_1, UGUI_POS(145, 45, 125, 25));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_1, FONT_20);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_1, "2 PLAYER");
                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_2PlayerPair[1], OBJ_ID_2, UGUI_POS(145, 75, 125, 15));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_2, FONT_12);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_2, "START");

                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_3PlayerPair[0], OBJ_ID_3, UGUI_POS(145, 95, 125, 25));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_3, FONT_20);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_3, "3 PLAYER");
                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_3PlayerPair[1], OBJ_ID_4, UGUI_POS(145, 125, 125, 15));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_4, FONT_12);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_4, "START");

                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_4PlayerPair[0], OBJ_ID_5, UGUI_POS(145, 145, 125, 25));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_5, FONT_20);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_5, "4 PLAYER");
                UG_TextboxCreate(&PlayerSelect_window, &PlayerSelect_4PlayerPair[1], OBJ_ID_6, UGUI_POS(145, 175, 125, 15));
                UG_TextboxSetFont(&PlayerSelect_window, OBJ_ID_6, FONT_12);
                UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_6, "START");

                UG_ButtonCreate(&PlayerSelect_window, &PlayerSelect_back, OBJ_ID_7, UGUI_POS(10, 200, 120, 16));
                UG_ButtonSetFont(&PlayerSelect_window, OBJ_ID_7, FONT_12);
                UG_ButtonSetText(&PlayerSelect_window, OBJ_ID_7, "<- BACK");
                UG_ButtonSetAlignment(&PlayerSelect_window, OBJ_ID_7, ALIGN_TOP_CENTER);

            case MENU_CARD_LOAD:
                UG_WindowCreate(&CardLoad_window, CardLoad_objs, CardLoad_objectCount, CardLoad_shapeDraw);
                UG_WindowSetStyle(&CardLoad_window, WND_STYLE_HIDE_TITLE);

                UG_TextboxCreate(&CardLoad_window, &CardLoad_gameTitle, OBJ_ID_0, UGUI_POS(65, 0, 285, 25));
                UG_TextboxSetFont(&CardLoad_window, OBJ_ID_0, FONT_20);
                UG_TextboxSetText(&CardLoad_window, OBJ_ID_0, "");

                UG_TextboxCreate(&CardLoad_window, &CardLoad_subtitle, OBJ_ID_1, UGUI_POS(65, 35, 285, 15));
                UG_TextboxSetFont(&CardLoad_window, OBJ_ID_1, FONT_12);
                UG_TextboxSetText(&CardLoad_window, OBJ_ID_1, "CARD LOAD");

                UG_TextboxCreate(&CardLoad_window, &CardLoad_instruction, OBJ_ID_2, UGUI_POS(40, 110, 335, 25));
                UG_TextboxSetFont(&CardLoad_window, OBJ_ID_2, FONT_20);
                UG_TextboxSetText(&CardLoad_window, OBJ_ID_2, "");

                UG_ProgressCreate(&CardLoad_window, &CardLoad_progress, OBJ_ID_3, UGUI_POS(135, 160, 145, 15));
                UG_ProgressSetProgress(&CardLoad_window, OBJ_ID_3, 0);

                UG_ButtonCreate(&CardLoad_window, &CardLoad_back, OBJ_ID_4, UGUI_POS(10, 200, 120, 16));
                UG_ButtonSetFont(&CardLoad_window, OBJ_ID_4, FONT_12);
                UG_ButtonSetText(&CardLoad_window, OBJ_ID_4, "EXIT (O)");
                UG_ButtonSetAlignment(&CardLoad_window, OBJ_ID_4, ALIGN_TOP_CENTER);

                break;

            case MENU_GAME:
                Game_constructMenu();

                break;

            default:
                break;
        }
    }
}

void GUI_setMenu(MenuScreen menu);
void GUI_updateCurrentMenu()
{
    DealData dealData;

    switch(GUI_currentMenu)
    {
        case MENU_MAIN:
            UG_WindowShow(&MainMenu_window);
            break;
        case MENU_GAME_SELECT:
            UG_WindowShow(&GameSelect_window);
            break;
        case MENU_PLAYER_SELECT:
            PlayerSelect_selectPlayerCount(PlayerSelect_players);
            UG_WindowShow(&PlayerSelect_window);
            break;
        case MENU_CARD_LOAD:
            dealData = Game_getDealData();

            if (dealData.cardsLoaded >= dealData.numCards)
            {
                GUI_setMenu(MENU_GAME);
                return;
            }

            UG_TextboxSetText(&CardLoad_window, OBJ_ID_2, CardLoad_strings[dealData.currPlayer - 1][dealData.currCard - 1]);
            UG_ProgressSetProgress(&CardLoad_window, OBJ_ID_3, (100 * dealData.cardsLoaded) / dealData.numCards);

            UG_WindowShow(&CardLoad_window);
            break;
        case MENU_GAME:
            Game_updateMenu();
            break;
        default:
            UG_FillScreen(C_WHITE); // TODO: We could add a driver for fill screen (Memset)
            break;
    }

    UG_Update();
}

void GUI_setMenu(MenuScreen menu)
{
    switch(menu)
    {
        case MENU_MAIN:
            GUI_highlightButton(&MainMenu_window, OBJ_ID_1);
            GUI_unhighlightButton(&MainMenu_window, OBJ_ID_2);

            GUI_selectedButton = 0;

            break;
        case MENU_GAME_SELECT:
            GUI_selectedButton = 0;
            GameSelect_selectedGame = 0;
            GUI_unhighlightButton(&GameSelect_window, OBJ_ID_4);

            UG_TextboxSetText(&GameSelect_window, OBJ_ID_2, Games_names[GameSelect_selectedGame]);

            break;
        case MENU_PLAYER_SELECT:
            PlayerSelect_players = 2;
            GUI_selectedButton = 0;
            GUI_selectedGame = GameSelect_selectedGame;

            UG_TextboxSetText(&PlayerSelect_window, OBJ_ID_0, Games_names[GUI_selectedGame]);
            GUI_unhighlightButton(&PlayerSelect_window, OBJ_ID_7);
            break;

        case MENU_CARD_LOAD:
            // We've selected the game, lets set it up
            Game_setMenu(GUI_selectedGame, PlayerSelect_players);

            UG_TextboxSetText(&CardLoad_window, OBJ_ID_0, Games_names[GUI_selectedGame]);

            GUI_unhighlightButton(&CardLoad_window, OBJ_ID_4);
            break;

        case MENU_GAME:
            // Game set menu is done above
            // TODO: Seperate Game_setMenu and Game_init
            break;
        default:
            break;
    }

    GUI_currentMenu = menu;
    GUI_updateCurrentMenu();
}

bool GUI_cardTap(uint32_t UUID)
{
    bool ret = false;

    switch (GUI_currentMenu)
    {
        case MENU_CARD_LOAD:
            ret = Game_registerCard(UUID);
            break;
        case MENU_GAME:
            Game_playCard(UUID);
            ret = true;
            break;
        default:
            break;
    }

    return ret;
}

void GUI_buttonCallback(ButtonHandle button, PressType type)
{
    switch (GUI_currentMenu)
    {
        case MENU_MAIN:
            if (BUTTON(RIGHT, SINGLE) && GUI_selectedButton == 0)
            {
                GUI_highlightButton(&MainMenu_window, OBJ_ID_2);
                GUI_unhighlightButton(&MainMenu_window, OBJ_ID_1);

                GUI_selectedButton = 1;
                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(LEFT, SINGLE) && GUI_selectedButton == 1)
            {
                GUI_highlightButton(&MainMenu_window, OBJ_ID_1);
                GUI_unhighlightButton(&MainMenu_window, OBJ_ID_2);

                GUI_selectedButton = 0;

                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(A, SINGLE))
            {
                if (GUI_selectedButton == 0)
                {
                    GUI_setMenu(MENU_GAME_SELECT);
                    break;
                }
            }
            break;

        case MENU_GAME_SELECT:

            if (BUTTON(UP, SINGLE) && GUI_selectedButton == 1)
            {
                GUI_selectedButton = 0;
                GUI_unhighlightButton(&GameSelect_window, OBJ_ID_4);


                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(DOWN, SINGLE) && GUI_selectedButton == 0)
            {
                GUI_selectedButton = 1;
                GUI_highlightButton(&GameSelect_window, OBJ_ID_4);


                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(LEFT, SINGLE) && GUI_selectedButton == 0)
            {
                if (GameSelect_selectedGame == 0)
                    GameSelect_selectedGame = GAME_COUNT;

                GameSelect_selectedGame--;

                UG_TextboxSetText(&GameSelect_window, OBJ_ID_2, Games_names[GameSelect_selectedGame]);

                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(RIGHT, SINGLE) && GUI_selectedButton == 0)
            {
                GameSelect_selectedGame++;
                if (GameSelect_selectedGame == GAME_COUNT)
                    GameSelect_selectedGame = 0;

                UG_TextboxSetText(&GameSelect_window, OBJ_ID_2, Games_names[GameSelect_selectedGame]);

                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(A, SINGLE))
            {
                if (GUI_selectedButton == 1)
                {
                    GUI_setMenu(MENU_MAIN);
                    break;
                }

                if (GUI_selectedButton == 0)
                {
                    GUI_setMenu(MENU_PLAYER_SELECT);
                    break;
                }
            }

            break;
        case MENU_PLAYER_SELECT:

            if (BUTTON(DOWN, SINGLE) && GUI_selectedButton < 3)
            {
                GUI_selectedButton++;
                PlayerSelect_players++;

                if (GUI_selectedButton == 3)
                    GUI_highlightButton(&PlayerSelect_window, OBJ_ID_7);

                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(UP, SINGLE) && GUI_selectedButton > 0)
            {
                GUI_selectedButton--;
                PlayerSelect_players--;

                GUI_unhighlightButton(&PlayerSelect_window, OBJ_ID_7);

                GUI_updateCurrentMenu();
                break;
            }

            if (BUTTON(A, SINGLE))
            {
                if (GUI_selectedButton < 3)
                {
                    // Player number selected
                    GUI_setMenu(MENU_CARD_LOAD);
                    break;
                }
                else
                {
                    // Return back
                    break;
                }
            }

            break;

        case MENU_CARD_LOAD:

            // if (BUTTON(A, SINGLE))
            // {
            //     GUI_setMenu(MENU_GAME);
            // }

            break;

        case MENU_GAME:
            Game_buttonCallback(button, type);
            break;
        default:
            break;
    }
}

static void GUI_init()
{
    UG_Init(&gui, &device);

    GUI_constructMenus();

    // Game_setMenu(GAME_HANABI, 2);
    // GUI_selectedGame = GAME_CARDSVSHUMANITY;
    // PlayerSelect_players = 2;

    // GUI_setMenu(MENU_CARD_LOAD);

    GUI_setMenu(MENU_MAIN);

    return;
}

extern bool guiNeedsUpdated;
bool guiNeedsUpdated = false;

void GUI_task(void *args)
{
    (void) args;

    GUI_init();

    for (;;)
    {
        for (ButtonHandle handle = 0; handle < BUTTON_COUNT; handle++)
        {
            if (buttons[handle].pressed != BUTTON_NOT_PRESSED)
            {
                buttons[handle].pressed = BUTTON_NOT_PRESSED;
                GUI_buttonCallback(handle, PRESS_SINGLE);
            }
        }

        if (guiNeedsUpdated)
        {
            GUI_updateCurrentMenu();
            guiNeedsUpdated = false;
        }

        Watchdog_tickle(THREAD_GUI);
        osDelay(THREAD_GUI_PERIOD);
    }
}
