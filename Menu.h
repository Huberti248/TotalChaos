#pragma once
#include <SDL.h>

#define MAIN_MENU_COLOR {0, 0, 100}
#define MAIN_MENU_BUTTON_PADDING 25

enum class MenuName {
	Play = 1,
	Quit
};

void CalculateButtonPosition(
	SDL_FRect* dstButton, 
	const int index, 
	const int numButtons, 
	const float width, 
	const float height, 
	float paddingVertical = 0);



