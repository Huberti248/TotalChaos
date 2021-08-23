#include "Movement.h"
//Helper file for definitions of the movement functions

void RotateEntityTowards(SDL_Texture* entTexture, const Entity& entToRotate, const SDL_Point& targetPos, SDL_Renderer* renderer) {
	float angle = MathUtils::GetAngle(targetPos.x, targetPos.y, entToRotate.r.x, entToRotate.r.y);

	int result = SDL_RenderCopyExF(renderer, entTexture, NULL, &entToRotate.r, angle, NULL, SDL_FLIP_NONE);
	
	#ifdef _DEBUG
	if (result != 0)
		std::cout << SDL_GetError() << std::endl;
	#endif
}
