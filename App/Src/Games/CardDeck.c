#include "Games/CardDeck.h"

#include "games.h"
#include "gui.h"
#include "ugui.h"

#include <rng.h>

UG_WINDOW CardDeck_window;

UG_TEXTBOX CardDeck_instructions[3];

typedef enum
{
    OBJ_SHUFFLE,
    OBJ_DISCARD,
    OBJ_EXIT,

    OBJ_COUNT
} CardDeck_objIds;
UG_OBJECT CardDeck_objs[OBJ_COUNT];

#define CARD_DECK_NUM_SUIT_CARDS    (1 + 10 + 3)
#define CARD_DECK_SIZE              (4 * CARD_DECK_NUM_SUIT_CARDS)

#define CARD_DECK_MAX_INPLAY_CARDS  25

typedef enum
{
    IN_DECK = 0U,
    DECK_DISCARD,
    DECK_HAND,
} Deck;

struct  {
    struct {
        uint8_t num;
        uint32_t uuid;
    } playerCards[CARD_DECK_MAX_INPLAY_CARDS];

    Deck deck[CARD_DECK_SIZE];
    uint8_t deckSize;

    uint32_t random;
} CardDeck_game;

/***************************************
            Game Logic
****************************************/

 static uint32_t CardDeck_proceduralRandomness()
 { // https://gist.github.com/badboy/6267743, Robert Jenkins' 32 bit integer hash function
     uint32_t num = CardDeck_game.random;
     num = (num + 0x7ed55d16) + (num << 12);
     num = (num ^ 0xc761c23c) ^ (num >> 19);
     num = (num + 0x165667b1) + (num << 5);
     num = (num + 0xd3a2646c) ^ (num << 9);
     num = (num + 0xfd7046c5) + (num << 3);
     num = (num ^ 0xb55a4f09) ^ (num >> 16);
     CardDeck_game.random = num;
     return num;
}

static void CardDeck_gameInit()
{
    memset(&CardDeck_game, 0U, sizeof(CardDeck_game));
    CardDeck_game.deckSize = CARD_DECK_SIZE;


    uint32_t random = 0;
    while (HAL_RNG_GenerateRandomNumber(&hrng, &random) != HAL_OK)
    {
        osDelay(5);
    }
    (void) CardDeck_proceduralRandomness();
}

DealData CardDeck_getDealData(void)
{
    DealData data = {
        .cardsLoaded = 0,
        .numCards = 0,
        .currCard = 0,
        .currPlayer = 0,
    };

    return data;
}

void CardDeck_playCard(uint32_t UUID)
{
    // If this card had data, put the card in the discard pile
    uint8_t playerIndex = 0;
    for (uint8_t card = 0; card < CARD_DECK_MAX_INPLAY_CARDS; card++)
    {
        if (CardDeck_game.playerCards[card].uuid == UUID)
        {
            CardDeck_game.deck[CardDeck_game.playerCards[card].num] = DECK_DISCARD;
            playerIndex = card;
            break;
        }

        if (CardDeck_game.playerCards[card].uuid == 0)
        {
            CardDeck_game.playerCards[card].uuid = UUID;
            playerIndex = card;
            break;
        }
    }

    // If the current deck is empty, put everything back in deck
    if (CardDeck_game.deckSize == 0)
    {
        for (uint8_t card = 0; card < CARD_DECK_SIZE; card++)
        {
            if (CardDeck_game.deck[card] == DECK_DISCARD)
            {
                CardDeck_game.deck[card] = IN_DECK;
                CardDeck_game.deckSize++;
            }
        }
    }

    // Pick a random card and assign it to the card
    uint8_t selected = CardDeck_proceduralRandomness() % CardDeck_game.deckSize;
    uint8_t index = 0;
    do
    {
        // Find the next in deck card
        for (uint8_t card = index + 1; card < CARD_DECK_SIZE; card++)
        {
            if (CardDeck_game.deck[card] == IN_DECK)
            {
                index = card;
                break;
            }
        }

        selected--;
    }
    while ( selected > 0 );

    CardDeck_game.deck[index] = DECK_HAND;
    CardDeck_game.playerCards[playerIndex].num = index;
}

SenderDataSpec CardDeck_sendCard(uint32_t UUID)
{
    // Find the card and send a Suit Card command
}

/***************************************
            GUI
****************************************/
void CardDeck_constructMenu(void)
{

}

void CardDeck_setMenu(uint8_t numPlayers)
{
    // Game Init
    CardDeck_gameInit(numPlayers);
}


void CardDeck_updateMenu(void)
{

}

void CardDeck_buttonCallback(ButtonHandle button, PressType type)
{
    if (BUTTON(A, SINGLE))
    {
        // Discard all cards and shuffle deck
    }

    if (BUTTON(B, SINGLE))
    {
        // Exit the game

    }
}