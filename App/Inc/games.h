#pragma once

#include "board.h"

#include "button.h"
#include "sender.h"

typedef enum
{
    GAME_POKER = 0,
    GAME_HANABI,
    GAME_CARDSVSHUMANITY,

    GAME_COUNT
} Games;

typedef struct
{
    uint8_t numCards;
    uint8_t cardsLoaded; // Total cards loaded (For progress)
    uint8_t currPlayer; // Current loading player
    uint8_t currCard; // Current loading card
} DealData;

extern char Games_names[GAME_COUNT][20];

/*
 * Required game functions:
 * Each game should implement this set of functions in some way. The functions should bear
 * the same name but replacing the Game_ prefix with the specific game's name (For e.g. Poker_setMenu)
 *      void Game_constructMenu(void) -
 *          - Called to construct/create the UGUI objects for the controller display
 *      void Game_setMenu(uint8_t selectedGame, uint8_t numPlayers)
 *          - Called once at game selection to initialize the game state
 *      void Game_updateMenu(void)
 *          - Function callback for updating the controller (Called whenever GUI_updateCurrentMenu is called)
 *          - This should populate variable fields in the display with the correct game data
 *      void Game_buttonCallback(ButtonHandle button, PressType type)
 *          - Called on button press with the pressed button and the type of press *
 *      DealData Game_getDealData(void)
 *          - Called after every card tap during card load to determine load progress
 *      void Game_registerCard(uint32_t UUID)
 *          - Called at every card tap during card load to register a tapped card to the game
 *      void Game_playCard(uint32_t UUID)
 *          - Called at every card tap during gameplay to register a game event
 *      SenderDataSpec Game_sendCard(uint32_t UUID)
 *          - Returns the relevant data for updating a card display based on its state in the game
 */

void Game_constructMenu(void);
void Game_setMenu(uint8_t selectedGame, uint8_t numPlayers);
void Game_updateMenu(void);
void Game_buttonCallback(ButtonHandle button, PressType type);

DealData Game_getDealData(void);
void Game_registerCard(uint32_t UUID);
void Game_playCard(uint32_t UUID);
SenderDataSpec Game_sendCard(uint32_t UUID);
