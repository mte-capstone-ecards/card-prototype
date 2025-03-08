#pragma once

#include "button.h"
// Hanabi_drawCard
// Hanabi_discardCard
// Hanabi_playCard

void Hanabi_constructMenu(void);
void Hanabi_setMenu(void);
void Hanabi_updateMenu(void);
void Hanabi_buttonCallback(ButtonHandle button, PressType type);
