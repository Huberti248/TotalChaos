#include "Movement.h"
//Helper file for definitions of the movement functions


void PlayerRotation(SDL_Texture* playerT, const Entity& player, const SDL_Point& mousePos, SDL_Renderer* renderer) {
	//SDL_Rect destinationRect;
	//
	//destinationRect.x = player.r.x;
	//destinationRect.y = player.r.y;

	//SDL_QueryTexture(playerT, NULL, NULL, &destinationRect.w, &destinationRect.h);
	//destinationRect.x -= (destinationRect.w / 2);
	//destinationRect.y -= (destinationRect.h / 2);

	float angle = MathUtils::GetAngle(mousePos.x, mousePos.y, player.r.x, player.r.y);


	int result = SDL_RenderCopyExF(renderer, playerT, NULL, &player.r, angle, NULL, SDL_FLIP_NONE);
	
	if (result != 0)
		std::cout << SDL_GetError() << std::endl;
}
