#pragma once
#include <SDL.h>
#include <iostream>
#include "Entity.h"
#include "MathUtils.h"

/// <summary>
/// Rotates the player by the given angle 
/// </summary>
/// <param name="mousePos"></param>
/// <param name="currPos"></param>
/// <param name="angle"></param>
void PlayerRotation(SDL_Texture* playerT, const Entity& player, const SDL_Point& mousePos, SDL_Renderer* renderer);
