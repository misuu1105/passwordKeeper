// Standard libraries
#include <iostream>
#include <vector>
#include <stdexcept>

// External libraies
#include <SDL2/SDL.h>
#include "UI.h"

const char* APP_NAME = "Password-Keeper";

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not be initialized: " << SDL_GetError() << '\n';
        std::exit(-1);
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf could not be initialized: " << TTF_GetError() << '\n';
        SDL_Quit();
        std::exit(-1);
    }

    SDL_Window* window = SDL_CreateWindow
    (
        APP_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
        TTF_Quit();
        SDL_Quit();
        std::exit(-1);
    }

    UI userInterfaceHandler{window};
    userInterfaceHandler.runApplication();
}
