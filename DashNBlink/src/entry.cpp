#include "game.h"

#include <iostream>

static bool s_running = true;

int main()
{
    std::cout << "Game starting\n" << std::endl;

    if (!game_init())
    {
        std::cout << "\nFailed to initialize game, closing :(...\n" << std::endl;
        return 1;
    }

   

    std::cout << "\nRunning game...\n" << std::endl;

    game_run();

    game_clean_up();

    std::cout << "\nGame closing\n" << std::endl;

    return 0;
}
