#pragma once
#include <SDL.h>
#include <iostream>
#include "../Entities/Entity.h"
#include "../Auxiliars/MathUtils.h"

void RotateEntityTowards(SDL_Texture* entTexture, const Entity& entToRotate, const SDL_Point& targetPos, SDL_Renderer* renderer);
