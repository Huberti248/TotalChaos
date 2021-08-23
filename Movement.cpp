#include "Movement.h"
//Helper file for definitions of the movement functions

void PlayerRotation(SDL_Texture* playerT, const Entity& player, const SDL_Point& mousePos, SDL_Renderer* renderer) {
	float angle = MathUtils::GetAngle(mousePos.x, mousePos.y, player.r.x, player.r.y);

	int result = SDL_RenderCopyExF(renderer, playerT, NULL, &player.r, angle, NULL, SDL_FLIP_NONE);
	
	#ifdef _DEBUG
	if (result != 0)
		std::cout << SDL_GetError() << std::endl;
	#endif
}
