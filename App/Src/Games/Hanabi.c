#include "Games/Hanabi.h"

#include "games.h"
#include "gui.h"
#include "ugui.h"
#include "ugui_fonts.h"

#include <string.h>
#include <rng.h>
#include <cmsis_os.h>

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
    CARD_PLAY,
    CARD_DISCARD,

    CARD_MODE_COUNT
} CardMode;

char *Hanabi_cardModes[CARD_MODE_COUNT] = {
    [CARD_PLAY] = "PLAY",
    [CARD_DISCARD] = "DISCARD",
};

typedef enum
{
    SHAPE_STAR,
    SHAPE_PLUS,
    SHAPE_DOLLAR,
    SHAPE_HASHTAG,
    SHAPE_AT,

    SHAPE_COUNT,
} Hanabi_ShapesDef;

char *Hanabi_shapes[SHAPE_COUNT] = {
    [SHAPE_STAR]    = "*",
    [SHAPE_PLUS]    = "+",
    [SHAPE_DOLLAR]  = "$",
    [SHAPE_HASHTAG] = "#",
    [SHAPE_AT]      = "@",
};

// Max num is determined by the number of clues (Lives -> 3, Cards -> 5, Clues -> 8)
#define HANABI_MAX_NUM 8
char *Hanabi_nums[1 + HANABI_MAX_NUM] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8"
};

enum
{
    HANABI_PLAYER_1,
    HANABI_PLAYER_2,
    HANABI_PLAYER_3,
    HANABI_PLAYER_4,

    HANABI_MAX_PLAYERS,
};

char *Hanabi_turnMsg[HANABI_MAX_PLAYERS] = {
    "PLAYER 1 TURN",
    "PLAYER 2 TURN",
    "PLAYER 3 TURN",
    "PLAYER 4 TURN",
};

typedef struct
{
    uint32_t UUID;

    uint8_t num;
    Hanabi_ShapesDef shape;
} Hanabi_card;

#define HANABI_NUM_SHAPE_CARDS  (2 + 2 + 2 + 2+ 1)
#define HANABI_DECK_SIZE        (SHAPE_COUNT * HANABI_NUM_SHAPE_CARDS)

struct {
    uint8_t lives;
    uint8_t clues;
    uint8_t table[SHAPE_COUNT]; // Placed cards

    CardMode cardMode;

    uint8_t turn;
    uint8_t numPlayers;

    uint8_t selectedButton;

    DealData dealData;
    uint8_t numPlayerCards;
    Hanabi_card playerCards[HANABI_MAX_PLAYERS][5];

    uint8_t topDeck;
    Hanabi_card deck[HANABI_DECK_SIZE];
} Hanabi_game;

#define HANABI_MAX_CLUES    8

/***************************************
            Game Logic
****************************************/

static uint32_t Hanabi_proceduralRandomness(uint32_t num)
{ // https://gist.github.com/badboy/6267743, Robert Jenkins' 32 bit integer hash function
    num = (num + 0x7ed55d16) + (num << 12);
    num = (num ^ 0xc761c23c) ^ (num >> 19);
    num = (num + 0x165667b1) + (num << 5);
    num = (num + 0xd3a2646c) ^ (num << 9);
    num = (num + 0xfd7046c5) + (num << 3);
    num = (num ^ 0xb55a4f09) ^ (num >> 16);
    return num;
}

static void Hanabi_setupDeck(void)
{
    // Initialize to no drawn cards
    Hanabi_game.topDeck = 0;

    uint32_t random = 0;
    while (HAL_RNG_GenerateRandomNumber(&hrng, &random) != HAL_OK)
    {
        osDelay(5);
    }
    random = Hanabi_proceduralRandomness(random);

    // Initially populate the deck deterministically
    uint8_t deal = 0;
    for (Hanabi_ShapesDef shape = 0; shape < SHAPE_COUNT; shape++)
    {
        for (uint8_t num = 0; num < HANABI_NUM_SHAPE_CARDS; num++)
        {
            Hanabi_game.deck[deal].shape = shape;
            Hanabi_game.deck[deal].num = 1 + (num >> 1);

            deal++;
        }
    }

    // Perform a Fisher-Yates shuffle
    Hanabi_card tmp;
    uint8_t cardA, cardB;
    for (uint16_t cardA = HANABI_DECK_SIZE - 1; cardA > 0; cardA--)
    {
        cardB = random % (cardA + 1);

        tmp = Hanabi_game.deck[cardA];
        Hanabi_game.deck[cardA] = Hanabi_game.deck[cardB];
        Hanabi_game.deck[cardB] = tmp;

        random = Hanabi_proceduralRandomness(random);
    }
}

static void Hanabi_gameInit(uint8_t numPlayers)
{
    memset(&Hanabi_game, 0U, sizeof(Hanabi_game));

    Hanabi_game.lives = 3;
    Hanabi_game.clues = HANABI_MAX_CLUES;

    Hanabi_game.numPlayers = numPlayers;
    Hanabi_game.numPlayerCards = numPlayers >= 4 ? 4 : 5;

    Hanabi_game.selectedButton = 2; // Start with selecting the give clue button

    // Deal data
    Hanabi_game.dealData.numCards = Hanabi_game.numPlayers * Hanabi_game.numPlayerCards;
    Hanabi_game.dealData.cardsLoaded = 0;
    Hanabi_game.dealData.currCard = 1;
    Hanabi_game.dealData.currPlayer = 1;

    // Create deck
    Hanabi_setupDeck();
}

static Hanabi_card Hanabi_dealCard(void)
{
    // TODO: Deal with empty deck
    return Hanabi_game.deck[Hanabi_game.topDeck++];
}

static void Hanabi_endTurn()
{
    Hanabi_game.turn++;
    if (Hanabi_game.turn == Hanabi_game.numPlayers)
        Hanabi_game.turn = 0;
}

DealData Hanabi_getDealData(void)
{
    return Hanabi_game.dealData;
}

uint8_t *Hanabi_getTable(void)
{
    return Hanabi_game.table;
}

void Hanabi_registerCard(uint32_t UUID)
{
    Hanabi_game.playerCards[Hanabi_game.dealData.currPlayer - 1][Hanabi_game.dealData.currCard - 1] = Hanabi_dealCard();
    Hanabi_game.playerCards[Hanabi_game.dealData.currPlayer - 1][Hanabi_game.dealData.currCard - 1].UUID = UUID;
    Hanabi_game.dealData.cardsLoaded++;

    Hanabi_game.dealData.currCard++;

    // Roll over to the next player
    if (Hanabi_game.dealData.currCard > Hanabi_game.numPlayerCards)
    {
        Hanabi_game.dealData.currPlayer++;
        Hanabi_game.dealData.currCard = 1;
    }
}

static void Hanabi_privatePlayCard(uint8_t player, uint8_t card)
{
    if (player != Hanabi_game.turn)
    {
        return;
    }

    Hanabi_card playCard = Hanabi_game.playerCards[player][card];

    if (Hanabi_game.cardMode == CARD_DISCARD)
    {
        if (Hanabi_game.clues == HANABI_MAX_CLUES)
            return; // Can't discard when at max clues

        Hanabi_game.clues++;
    }
    else if (Hanabi_game.cardMode == CARD_PLAY)
    {
        if (playCard.num == (Hanabi_game.table[playCard.shape] + 1))
        {
            // Succesful play!
            Hanabi_game.table[playCard.shape]++;
        }
        else
        {
            // Unsucessful play!
            Hanabi_game.lives--;

            // TODO: Deal with no lives left
        }
    }

    // TODO: Deal with situation of last card in the deck
    Hanabi_card newCard = Hanabi_dealCard();
    Hanabi_game.playerCards[player][card].shape = newCard.shape;
    Hanabi_game.playerCards[player][card].num = newCard.num;
    Hanabi_endTurn();
}

void Hanabi_playCard(uint32_t UUID)
{
    for (uint8_t player = 0; player < Hanabi_game.numPlayers; player++)
    {
        for (uint8_t card = 0; card < Hanabi_game.numPlayerCards; card++)
        {
            if (Hanabi_game.playerCards[player][card].UUID == UUID)
            {
                Hanabi_privatePlayCard(player, card);
                return;
            }
        }
    }
}

SenderDataSpec Hanabi_sendCard(uint32_t UUID)
{
    SenderDataSpec data = { 0U };

    for (uint8_t player = 0; player < Hanabi_game.numPlayers; player++)
    {
        for (uint8_t card = 0; card < Hanabi_game.numPlayerCards; card++)
        {
            if (Hanabi_game.playerCards[player][card].UUID == UUID)
            {
                data.shape = Hanabi_game.playerCards[player][card].shape;
                data.num = Hanabi_game.playerCards[player][card].num;
            }
        }
    }

    return data;
}

/***************************************
            GUI
****************************************/
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
    UG_TextboxSetText(&Hanabi_window, OBJ_PLAYER_TURN, "");

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

    for (uint8_t card = 0; card < 5; card++)
    {
        UG_TextboxCreate(&Hanabi_window, &Hanabi_cardIndicator[card][0], OBJ_CARD0_NUM + 2 * card, UGUI_POS(34 + card * 83, 70, 20, 25));
        UG_TextboxSetFont(&Hanabi_window, OBJ_CARD0_NUM + 2 * card, FONT_20);
        UG_TextboxSetText(&Hanabi_window, OBJ_CARD0_NUM + 2 * card, "");

        UG_TextboxCreate(&Hanabi_window, &Hanabi_cardIndicator[card][1], OBJ_CARD0_SHAPE + 2 * card, UGUI_POS(34 + card * 83, 120, 20, 25));
        UG_TextboxSetFont(&Hanabi_window, OBJ_CARD0_SHAPE + 2 * card, FONT_20);
        UG_TextboxSetText(&Hanabi_window, OBJ_CARD0_SHAPE + 2 * card, Hanabi_shapes[card]);
    }

    UG_TextboxCreate(&Hanabi_window, &Hanabi_lives[0], OBJ_LIFE_LABEL, UGUI_POS(15, 170, 45, 15));
    UG_TextboxSetFont(&Hanabi_window, OBJ_LIFE_LABEL, FONT_12);
    UG_TextboxSetText(&Hanabi_window, OBJ_LIFE_LABEL, "LIVES");

    UG_TextboxCreate(&Hanabi_window, &Hanabi_lives[1], OBJ_LIFE_NUM, UGUI_POS(70, 160, 20, 25));
    UG_TextboxSetFont(&Hanabi_window, OBJ_LIFE_NUM, FONT_20);
    UG_TextboxSetText(&Hanabi_window, OBJ_LIFE_NUM, Hanabi_nums[3]);

    UG_TextboxCreate(&Hanabi_window, &Hanabi_clues[0], OBJ_CLUE_LABEL, UGUI_POS(355, 170, 45, 15));
    UG_TextboxSetFont(&Hanabi_window, OBJ_CLUE_LABEL, FONT_12);
    UG_TextboxSetText(&Hanabi_window, OBJ_CLUE_LABEL, "CLUES");

    UG_TextboxCreate(&Hanabi_window, &Hanabi_clues[1], OBJ_CLUE_NUM, UGUI_POS(335, 160, 20, 25));
    UG_TextboxSetFont(&Hanabi_window, OBJ_CLUE_NUM, FONT_20);
    UG_TextboxSetText(&Hanabi_window, OBJ_CLUE_NUM, "");

    UG_TextboxCreate(&Hanabi_window, &Hanabi_cardMode[0], OBJ_MODE_LABEL, UGUI_POS(175, 155, 65, 15));
    UG_TextboxSetFont(&Hanabi_window, OBJ_MODE_LABEL, FONT_12);
    UG_TextboxSetText(&Hanabi_window, OBJ_MODE_LABEL, "MODE");
    UG_TextboxSetBackColor(&Hanabi_window, OBJ_MODE_LABEL, C_NONE);

    UG_TextboxCreate(&Hanabi_window, &Hanabi_cardMode[1], OBJ_MODE_CURRENT, UGUI_POS(155, 160, 100, 25));
    UG_TextboxSetFont(&Hanabi_window, OBJ_MODE_CURRENT, FONT_20);
    UG_TextboxSetText(&Hanabi_window, OBJ_MODE_CURRENT, "");
    UG_TextboxSetBackColor(&Hanabi_window, OBJ_MODE_CURRENT, C_NONE);

}

void Hanabi_setMenu(uint8_t numPlayers)
{
    // Game Init
    Hanabi_gameInit(numPlayers);
}

void Hanabi_updateMenu(void)
{
    UG_TextboxSetText(&Hanabi_window, OBJ_PLAYER_TURN, Hanabi_turnMsg[Hanabi_game.turn]);
    UG_TextboxSetText(&Hanabi_window, OBJ_LIFE_NUM, Hanabi_nums[Hanabi_game.lives]);
    UG_TextboxSetText(&Hanabi_window, OBJ_CLUE_NUM, Hanabi_nums[Hanabi_game.clues]);

    for (uint8_t card = 0; card < SHAPE_COUNT; card++)
    {
        UG_TextboxSetText(&Hanabi_window, OBJ_CARD0_NUM + card * 2, Hanabi_nums[Hanabi_game.table[card]]);
    }

    UG_TextboxSetText(&Hanabi_window, OBJ_MODE_CURRENT, Hanabi_cardModes[Hanabi_game.cardMode]);

    for (uint8_t i = 0; i < 3; i++)
    {
        GUI_unhighlightButton(&Hanabi_window, OBJ_EXIT + i);
    }
    GUI_highlightButton(&Hanabi_window, OBJ_EXIT + Hanabi_game.selectedButton);

    UG_WindowShow(&Hanabi_window);
}

void Hanabi_buttonCallback(ButtonHandle button, PressType type)
{
    if (BUTTON(LEFT, SINGLE) && Hanabi_game.selectedButton > 0)
    {
        Hanabi_game.selectedButton--;
        GUI_updateCurrentMenu();
    }

    if (BUTTON(RIGHT, SINGLE) && Hanabi_game.selectedButton < 2)
    {
        Hanabi_game.selectedButton++;
        GUI_updateCurrentMenu();
    }

    if (BUTTON(A, SINGLE))
    {
        switch (Hanabi_game.selectedButton)
        {
            case 0:
                // TODO: Confirmation message
                // TODO: What menu do we go to if we exit the game
                break;
            case 1:
                Hanabi_game.cardMode ^= 1;
                GUI_updateCurrentMenu();
                break;
            case 2:
                if (Hanabi_game.clues > 0)
                {
                    Hanabi_game.clues--;
                    Hanabi_endTurn();
                    GUI_updateCurrentMenu();
                }
                break;
            default:
                break;
        }
    }
}


