#include "sim.h"

#include "Hanabi.h"

#include <time.h>
#include <stdlib.h>

#define NUM_PLAYERS 3
#define NUM_PLAYER_CARDS    (NUM_PLAYERS >=  4 ? 4 : 5)
#define NUM_CARDS  (NUM_PLAYERS * NUM_PLAYER_CARDS)
uint32_t cards[NUM_CARDS];

char *shapes[] = {
    "*",
    "+",
    "$",
    "#",
    "@",
};

static void printTable(void)
{
    uint8_t *table = Hanabi_getTable();
    for (uint8_t i = 0; i < 5; i++)
    {
        printf("%s - %d\t", shapes[i], table[i]);
    }
    printf("\n");
}

int main()
{
    srand(0);

    // Set up the dummy card UUIDs
    for (uint8_t card = 0; card < NUM_CARDS; card++)
    {
        cards[card] = rand();
    }

    // Initialize the game
    Hanabi_setMenu(NUM_PLAYERS);

    /* Card Load */

    // Perform a tap of the cards in order - this is dealing the cards
    SenderDataSpec spec;
    DealData dealData = Hanabi_getDealData();
    uint8_t card = 0;
    while (dealData.cardsLoaded < dealData.numCards)
    {
        Hanabi_registerCard(cards[card]);
        spec = Hanabi_sendCard(cards[card]);
        printf("Player %d Card %d: \t%s%d\n", dealData.currPlayer, dealData.currCard, shapes[spec.shape], spec.num);

        card++;
        dealData = Hanabi_getDealData();
    }

    Hanabi_playCard(cards[1]); // Play @1
    printTable();
    spec = Hanabi_sendCard(cards[1]);
    printf("New Card: \t%s%d\n", shapes[spec.shape], spec.num);

    // Try to play another player 1 card on player 2 turn
    Hanabi_playCard(cards[4]); // *1
    printTable();
    Hanabi_playCard(cards[8]); // @2
    printTable();

    Hanabi_playCard(cards[13]); // +2 by p3 - fail
    printTable();

    Hanabi_playCard(cards[4]); // *1 by p1
    printTable();

    return 0;
}
