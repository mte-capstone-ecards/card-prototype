#include "games.h"

#include "Games/Hanabi.h"
#include "Games/Humanity.h"

char Games_names[GAME_COUNT][20] = {
    // [GAME_POKER] = "POKER",
    [GAME_HANABI] = "HANABI",
    [GAME_CARDSVSHUMANITY] = "CARDS VS HUMANITY",
};

static uint8_t Game_selectedGame = 0;

void Game_constructMenu(void)
{
    for (Games game = 0; game < GAME_COUNT; game++)
    {
        switch (game)
        {
            case GAME_HANABI:
                Hanabi_constructMenu();
                case GAME_CARDSVSHUMANITY:
                    Humanity_constructMenu();
            default:
                break;
        }
    }
}

void Game_setMenu(uint8_t selectedGame, uint8_t numPlayers)
{
    Game_selectedGame = selectedGame;

    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            Hanabi_setMenu(numPlayers);
        case GAME_CARDSVSHUMANITY:
            Humanity_setMenu(numPlayers);
        default:
            break;
    }
}
void Game_updateMenu(void)
{
    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            Hanabi_updateMenu();
        case GAME_CARDSVSHUMANITY:
            Humanity_updateMenu();
        default:
            break;
    }
}

void Game_buttonCallback(ButtonHandle button, PressType type)
{
    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            Hanabi_buttonCallback(button, type);
        case GAME_CARDSVSHUMANITY:
            Humanity_buttonCallback(button, type);
        default:
            break;
    }
}

DealData Game_getDealData(void)
{
    DealData dealData;

    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            dealData = Hanabi_getDealData();
            break;
        case GAME_CARDSVSHUMANITY:
            dealData = Humanity_getDealData();
            break;
        default:
            break;
    }

    return dealData;
}

bool Game_registerCard(uint32_t UUID)
{
    bool ret = false;

    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            ret = Hanabi_registerCard(UUID);
            break;
        case GAME_CARDSVSHUMANITY:
            ret = Humanity_registerCard(UUID);
            break;
        default:
            break;
    }

    return ret;
}

void Game_playCard(uint32_t UUID)
{
    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            Hanabi_playCard(UUID);
            break;
        case GAME_CARDSVSHUMANITY:
            Humanity_playCard(UUID);
            break;
        default:
            break;
    }
}

SenderDataSpec Game_sendCard(uint32_t UUID)
{
    // Default to a clear command
    SenderDataSpec senderData = {
        .instr = SENDER_STRING_INSTR,
        .string = {
            .indices = { 0U },
            .len = 1,
            .str = { 0U },
        },
     };

    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            senderData = Hanabi_sendCard(UUID);
            break;
        case GAME_CARDSVSHUMANITY:
            senderData = Humanity_sendCard(UUID);
            break;
        default:
            break;
    }

    return senderData;
}

