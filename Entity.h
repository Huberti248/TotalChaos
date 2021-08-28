#pragma once
#include <SDL.h>

#include "SpawnPlace.h"

enum class PlanetType {
    Shield,
    Weapon,
};

//Parent entity class
class Entity {
public:
    SDL_FRect r{};
    float dx = 0;
    float dy = 0;
    PlanetType planetType = PlanetType::Shield;

    Entity();
    ~Entity();
};