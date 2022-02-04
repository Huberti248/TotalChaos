#pragma once
#include "../Engine/SystemImports.h"
#include "../Engine/SdlImports.h"
#include "../Engine/SdlUtils.h"
#include "../UI/Menu.h"
#include "../UI/UIControllers.h"
#include "../UI/ControlsMenu.h"

SDL_Window* window;

void logOutputCallback(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
    std::cout << message << std::endl;
}

float randomF(float M, float N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

std::ostream& operator<<(std::ostream& os, SDL_FRect r)
{
    os << r.x << " " << r.y << " " << r.w << " " << r.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, SDL_Rect r)
{
    SDL_FRect fR;
    fR.w = r.w;
    fR.h = r.h;
    fR.x = r.x;
    fR.y = r.y;
    os << fR;
    return os;
}