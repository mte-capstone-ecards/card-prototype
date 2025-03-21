#pragma once
#include "games.h"

#include "button.h"

#define NUM_BLACKS  100
#define NUM_WHITES  100

extern char blacks[NUM_BLACKS][4][21];
extern char whites[NUM_WHITES][4][21];


/*
 * Required game functions:
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

void Humanity_constructMenu(void);
void Humanity_setMenu(uint8_t numPlayers);
void Humanity_updateMenu(void);
void Humanity_buttonCallback(ButtonHandle button, PressType type);

DealData Humanity_getDealData(void);
bool Humanity_registerCard(uint32_t UUID);
void Humanity_playCard(uint32_t UUID);
SenderDataSpec Humanity_sendCard(uint32_t UUID);
