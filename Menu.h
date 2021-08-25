#pragma once
#include <SDL.h>

enum MenuName {
	Play = 1,
	Quit
};

void CalculateButtonPosition(SDL_FRect* dstButton, const int index, const int numButtons, const float width, const float height);

