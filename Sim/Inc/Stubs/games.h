#pragma once

#include <stdint.h>

#include "sender.h"
#include "button.h"

typedef struct
{
    uint8_t numCards;
    uint8_t cardsLoaded; // Total cards loaded (For progress)
    uint8_t currPlayer; // Current loading player
    uint8_t currCard; // Current loading card
} DealData;
