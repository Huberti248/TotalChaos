#include "Menu.h"
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