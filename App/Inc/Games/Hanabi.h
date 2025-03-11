#pragma once

#include "games.h"

#include "button.h"

// Hanabi_drawCard
// Hanabi_discardCard
// Hanabi_playCard

void Hanabi_constructMenu(void);
void Hanabi_setMenu(uint8_t numPlayers);
void Hanabi_updateMenu(void);
void Hanabi_buttonCallback(ButtonHandle button, PressType type);

DealData Hanabi_getDealData(void);
void Hanabi_registerCard(uint32_t UUID);
void Hanabi_playCard(uint32_t UUID);
SenderDataSpec Hanabi_sendCard(uint32_t UUID);
