#pragma once
#include <SDL.h>

//Parent entity class
class Entity {
public:
	SDL_FRect r{};
	float dx;
	float dy;

	Entity();
	~Entity();

};