#include "games.h"

#include "Games/Hanabi.h"

char Games_names[GAME_COUNT][20] = {
    [GAME_POKER] = "POKER",
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
            default:
                break;
        }
    }
}

void Game_setMenu(uint8_t selectedGame)
{
    Game_selectedGame = selectedGame;

    switch (Game_selectedGame)
    {
        case GAME_HANABI:
            Hanabi_setMenu();
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
        default:
            break;
    }
}

