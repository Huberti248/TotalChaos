#pragma once
#include <SDL.h>
#include <iostream>
#include "Entity.h"
#include "MathUtils.h"

void RotateEntityTowards(SDL_Texture* playerT, const Entity& player, const SDL_Point& mousePos, SDL_Renderer* renderer);
