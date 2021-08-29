#pragma once
#include <SDL.h>

#define LETTER_WIDTH 20
#define POINTER_OFFSET 30
#define BUTTON_SELECTED {200, 200, 0}
#define BUTTON_UNSELECTED {255, 255, 255}
#define TITLE_COLOR {200, 0, 0}

#define MAIN_MENU_BUTTON_PADDING 10

#define PAUSE_NUM_OPTIONS 3
#define PAUSE_MENU_BUTTON_PADDING 15

#define GAMEOVER_NUM_OPTIONS 2
#define GAMEOVER_MENU_BUTTON_PADDING 15

#define HIGHSCORE_WIDTH 280

enum class MenuOption {
	Play = 1,
	Resume,
	Restart,
	Controls,
	Highscores,
	Main,
	Credits,
	Quit
};

void CalculateButtonPosition(
	SDL_FRect* dstButton, 
	const int index, 
	const int numButtons, 
	const float width, 
	const float height, 
	float paddingVertical = 0);

void CalculateHighScorePosition(
	SDL_FRect* dstButton,
	SDL_FRect* scorePos,
	int index
);