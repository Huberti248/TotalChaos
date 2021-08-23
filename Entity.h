#pragma once
#include <SDL.h>

#include "SpawnPlace.h"

//Parent entity class
class Entity {
public:
	SDL_FRect r{};
	float dx = 0;
	float dy = 0;
	SpawnPlace spawnPlace = SpawnPlace::Up;

	Entity();
	~Entity();
};