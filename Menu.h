#pragma once
#include <SDL.h>

#define LETTER_WIDTH 20
#define POINTER_OFFSET 30

#define MAIN_MENU_COLOR {150, 0, 0}
#define MAIN_MENU_BUTTON_PADDING 25

#define PAUSE_NUM_OPTIONS 5
#define PAUSE_UNSELECTED { 255, 255, 255 }
#define PAUSE_SELECTED { 255, 0, 0 }
#define PAUSE_MENU_COLOR { 100, 0, 0 }
#define PAUSE_MENU_BUTTON_PADDING 15

#define GAMEOVER_NUM_OPTIONS 3
#define GAMEOVER_UNSELECTED { 255, 255, 255 }
#define GAMEOVER_SELECTED { 255, 0, 0 }
#define GAMEOVER_MENU_COLOR { 255, 0, 0 }
#define GAMEOVER_MENU_BUTTON_PADDING 15

enum class MenuOption {
	Play = 1,
	Resume,
	Restart,
	Controls,
	Highscores,
	Main,
	Quit
};

void CalculateButtonPosition(
	SDL_FRect* dstButton, 
	const int index, 
	const int numButtons, 
	const float width, 
	const float height, 
	float paddingVertical = 0);



