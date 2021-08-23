#pragma once
#include <SDL.h>

struct Entity {
    SDL_FRect r{};
    int dx = 0;
    int dy = 0;
};