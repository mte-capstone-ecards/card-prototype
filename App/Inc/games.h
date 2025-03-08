#pragma once

#include "Games/Hanabi.h"

typedef enum
{
    GAME_POKER = 0,
    GAME_HANABI,
    GAME_CARDSVSHUMANITY,

    GAME_COUNT
} Games;

extern char Games_names[GAME_COUNT][20];

void Game_constructMenu(void);
void Game_setMenu(uint8_t selectedGame);
void Game_updateMenu(void);
void Game_buttonCallback(ButtonHandle button, PressType type);
