#include "Menu.h"

void CalculateButtonPosition(SDL_FRect* dstButton, const int index, const int numButtons, const float width, const float height) {
	dstButton->x = width / 2.0f - dstButton->w / 2.0f;
	dstButton->y = height / 2.0f - numButtons * dstButton->h / 2.0f + (index + 0.5f) * dstButton->h;
}



