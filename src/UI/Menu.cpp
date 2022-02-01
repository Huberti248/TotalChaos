#include "Menu.h"
#include <stdio.h>
//Helper functions for menu.

void CalculateButtonPosition(
	SDL_FRect* dstButton,
	const int index,
	const int numButtons,
	const float width,
	const float height,
	float paddingVertical) {

	dstButton->x = width / 2.0f - dstButton->w / 2.0f;
	dstButton->y = height / 2.0f - numButtons * (dstButton->h / 2.0f + paddingVertical) + (index + 0.5f) * (dstButton->h + paddingVertical);
}

void CalculateHighScorePosition(
	SDL_FRect* dstButton,
	SDL_FRect* scorePos,
	int index
	) {

	switch (index) {
		case 0:
			dstButton->x = 640 - dstButton->w / 2.0f;
			dstButton->y = 150;
			scorePos->x = 640 - scorePos->w / 2.0f;
			scorePos->y = 275;
			break;
		case 1:
			dstButton->x = 640 + HIGHSCORE_WIDTH - dstButton->w / 2.0f;
			dstButton->y = 260;
			scorePos->x = 640 + HIGHSCORE_WIDTH - scorePos->w / 2.0f;
			scorePos->y = 375;
			break;
		case 2:
			dstButton->x = 640 - HIGHSCORE_WIDTH - dstButton->w / 2.0f;
			dstButton->y = 385;
			scorePos->x = 640 - HIGHSCORE_WIDTH - scorePos->w / 2.0f;
			scorePos->y = 505;
			break;
	}
}

