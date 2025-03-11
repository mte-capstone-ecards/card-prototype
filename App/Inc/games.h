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

void Game_constructMenu(void);
void Game_setMenu(uint8_t selectedGame, uint8_t numPlayers);
void Game_updateMenu(void);
void Game_buttonCallback(ButtonHandle button, PressType type);

DealData Game_getDealData(void);
void Game_registerCard(uint32_t UUID);
void Game_playCard(uint32_t UUID);
SenderDataSpec Game_sendCard(uint32_t UUID);
