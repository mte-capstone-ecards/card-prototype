#include "Games/Humanity.h"

#include "games.h"
#include "gui.h"
#include "ugui.h"

#include <string.h>
#include <rng.h>
#include <cmsis_os.h>

UG_WINDOW Humanity_window;

UG_TEXTBOX Humanity_prompts[4];

typedef enum
{
    OBJ_PROMPT0,
    OBJ_PROMPT1,
    OBJ_PROMPT2,
    OBJ_PROMPT3,

    OBJ_COUNT
} Humanity_objIds;
UG_OBJECT Humanity_objs[OBJ_COUNT];

#define Humanity_NUM_SHAPE_CARDS  (2 + 2 + 2 + 2+ 1)
#define Humanity_DECK_SIZE        (SHAPE_COUNT * Humanity_NUM_SHAPE_CARDS)

typedef struct
{
    uint32_t UUID;
    uint8_t random;
} Humanity_card;

struct {
    uint8_t numPlayers;

    DealData dealData;
    uint8_t numPlayerCards;
    Humanity_card playerCards[5][5];
    uint32_t random;
    uint8_t black;
} Humanity_game;

#define Humanity_MAX_CLUES    8

/***************************************
            Game Logic
****************************************/

static uint32_t Humanity_proceduralRandomness()
{ // https://gist.github.com/badboy/6267743, Robert Jenkins' 32 bit integer hash function
    uint32_t num = Humanity_game.random;
    num = (num + 0x7ed55d16) + (num << 12);
    num = (num ^ 0xc761c23c) ^ (num >> 19);
    num = (num + 0x165667b1) + (num << 5);
    num = (num + 0xd3a2646c) ^ (num << 9);
    num = (num + 0xfd7046c5) + (num << 3);
    num = (num ^ 0xb55a4f09) ^ (num >> 16);
    Humanity_game.random = num;
    return num;
}

uint8_t Humanity_dealBlack(void)
{
    return Humanity_proceduralRandomness() % NUM_BLACKS;
}

uint8_t Humanity_dealWhite(void)
{
    return Humanity_proceduralRandomness() % NUM_WHITES;
}

static void Humanity_gameInit(uint8_t numPlayers)
{
    memset(&Humanity_game, 0U, sizeof(Humanity_game));

    Humanity_game.numPlayers = numPlayers;
    Humanity_game.numPlayerCards = 3;

    // Deal data
    Humanity_game.dealData.numCards = Humanity_game.numPlayers * Humanity_game.numPlayerCards;
    Humanity_game.dealData.cardsLoaded = 0;
    Humanity_game.dealData.currCard = 1;
    Humanity_game.dealData.currPlayer = 1;

    uint32_t random = 0;
    while (HAL_RNG_GenerateRandomNumber(&hrng, &random) != HAL_OK)
    {
        osDelay(5);
    }
    (void) Humanity_proceduralRandomness();

    Humanity_game.black = Humanity_dealBlack();
}

DealData Humanity_getDealData(void)
{
    return Humanity_game.dealData;
}

bool Humanity_registerCard(uint32_t UUID)
{
    // Check this UUID is not already registered
    for (uint8_t player = 0; player < Humanity_game.numPlayers; player++)
    {
        for (uint8_t card = 0; card < Humanity_game.numPlayerCards; card++)
        {
            if (Humanity_game.playerCards[player][card].UUID == UUID)
            {
                return false;
            }
        }
    }

    if (Humanity_game.dealData.currPlayer > Humanity_game.numPlayers)
        Humanity_game.playerCards[Humanity_game.dealData.currPlayer - 1][Humanity_game.dealData.currCard - 1].random = Humanity_dealBlack();
    else
        Humanity_game.playerCards[Humanity_game.dealData.currPlayer - 1][Humanity_game.dealData.currCard - 1].random = Humanity_dealWhite();
    Humanity_game.playerCards[Humanity_game.dealData.currPlayer - 1][Humanity_game.dealData.currCard - 1].UUID = UUID;
    Humanity_game.dealData.cardsLoaded++;

    Humanity_game.dealData.currCard++;

    // Roll over to the next player
    if (Humanity_game.dealData.currCard > Humanity_game.numPlayerCards)
    {
        Humanity_game.dealData.currPlayer++;
        Humanity_game.dealData.currCard = 1;
    }

    return true;
}

static void Humanity_privatePlayCard(uint8_t player, uint8_t card)
{
    Humanity_card playCard = Humanity_game.playerCards[player][card];

    if (player > Humanity_game.numPlayers)
        Humanity_game.playerCards[player][card].random = Humanity_dealBlack();
    else
        Humanity_game.playerCards[player][card].random = Humanity_dealWhite();

}

void Humanity_playCard(uint32_t UUID)
{
    for (uint8_t player = 0; player < Humanity_game.numPlayers; player++)
    {
        for (uint8_t card = 0; card < Humanity_game.numPlayerCards; card++)
        {
            if (Humanity_game.playerCards[player][card].UUID == UUID)
            {
                Humanity_privatePlayCard(player, card);
                return;
            }
        }
    }
}

SenderDataSpec Humanity_sendCard(uint32_t UUID)
{
    // Default to a clear command
    SenderDataSpec data = {
        .instr = SENDER_STRING_INSTR,
        .string = {
            .indices = { 0U },
            .len = 1,
            .str = { 0U },
        },
    };

    for (uint8_t player = 0; player < Humanity_game.numPlayers + 1; player++)
    {
        for (uint8_t card = 0; card < Humanity_game.numPlayerCards; card++)
        {
            if (Humanity_game.playerCards[player][card].UUID == UUID)
            {
                data.instr = SENDER_STRING_INSTR;
                data.string.len = ((20 + 1) * 4) / 4;
                data.string.indices[0] = 0;
                data.string.indices[1] = 21;
                data.string.indices[2] = 42;
                data.string.indices[3] = 63;
                if (player > Humanity_game.numPlayers)
                {
                    // Black card
                    strcpy(&data.string.str[0], blacks[Humanity_game.playerCards[player][card].random][0]);
                    strcpy(&data.string.str[21], blacks[Humanity_game.playerCards[player][card].random][1]);
                    strcpy(&data.string.str[42], blacks[Humanity_game.playerCards[player][card].random][2]);
                    strcpy(&data.string.str[63], blacks[Humanity_game.playerCards[player][card].random][3]);
                }
                else
                {
                    strcpy(&data.string.str[0], whites[Humanity_game.playerCards[player][card].random][0]);
                    strcpy(&data.string.str[21], whites[Humanity_game.playerCards[player][card].random][1]);
                    strcpy(&data.string.str[42], whites[Humanity_game.playerCards[player][card].random][2]);
                    strcpy(&data.string.str[63], whites[Humanity_game.playerCards[player][card].random][3]);
                }
            }
        }
    }

    return data;
}

/***************************************
            GUI
****************************************/
void Humanity_shapeDraw(UG_MESSAGE* msg)
{
    UG_DrawLine(UGUI_POS(5, 200, 405, 0), C_BLACK);
}

void Humanity_constructMenu(void)
{
    UG_WindowCreate(&Humanity_window, Humanity_objs, OBJ_COUNT, Humanity_shapeDraw);
    UG_WindowSetStyle(&Humanity_window, WND_STYLE_HIDE_TITLE);

    UG_TextboxCreate(&Humanity_window, &Humanity_prompts[0], OBJ_PROMPT0, UGUI_POS(20, 40 + 30 * 0, 416 - 40, 25));
    UG_TextboxSetFont(&Humanity_window, OBJ_PROMPT0, FONT_16);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT0, "");

    UG_TextboxCreate(&Humanity_window, &Humanity_prompts[1], OBJ_PROMPT1, UGUI_POS(20, 40 + 30 * 1, 416 - 40, 25));
    UG_TextboxSetFont(&Humanity_window, OBJ_PROMPT1, FONT_16);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT1, "");

    UG_TextboxCreate(&Humanity_window, &Humanity_prompts[2], OBJ_PROMPT2, UGUI_POS(20, 40 + 30 * 2, 416 - 40, 25));
    UG_TextboxSetFont(&Humanity_window, OBJ_PROMPT2, FONT_16);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT2, "");

    UG_TextboxCreate(&Humanity_window, &Humanity_prompts[3], OBJ_PROMPT3, UGUI_POS(20, 40 + 30 * 3, 416 - 40, 25));
    UG_TextboxSetFont(&Humanity_window, OBJ_PROMPT3, FONT_16);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT3, "");
}

void Humanity_setMenu(uint8_t numPlayers)
{
    // Game Init
    Humanity_gameInit(numPlayers);
}

void Humanity_updateMenu(void)
{
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT0, blacks[Humanity_game.black][0]);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT1, blacks[Humanity_game.black][1]);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT2, blacks[Humanity_game.black][2]);
    UG_TextboxSetText(&Humanity_window, OBJ_PROMPT3, blacks[Humanity_game.black][3]);

    UG_WindowShow(&Humanity_window);
}

void Humanity_buttonCallback(ButtonHandle button, PressType type)
{
    if (BUTTON(A, SINGLE))
    {
        Humanity_game.black = Humanity_dealBlack();
        GUI_updateCurrentMenu();
    }
}


