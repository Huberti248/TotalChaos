#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "../Auxiliars/SpawnPlace.h"
#include "../Engine/Logger.h"
#include "../Engine/Clock.h"
#include "../DataManagement/TextureLoader.h"

//Parent entity class
class Entity {
public:
	
	SDL_FRect r{};
	float dx = 0;
	float dy = 0;

	bool operator ==(const Entity& other);

	Entity();
	~Entity();

	virtual SDL_Texture* GetTexture();
};
