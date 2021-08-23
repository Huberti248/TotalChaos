#pragma once
#include <SDL.h>
#include <iostream>
#include "Entity.h"
#include "MathUtils.h"

void RotateEntityTowards(SDL_Texture* entTexture, const Entity& entToRotate, const SDL_Point& targetPos, SDL_Renderer* renderer);
