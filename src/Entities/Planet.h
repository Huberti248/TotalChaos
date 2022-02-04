#pragma once
#include "Entity.h"
#include "../Auxiliars/MathUtils.h"
#include "../Engine/SdlImports.h"
#include "Player.h"

#define PLANET_SPEED 0.1

enum class PlanetType {
	Shield,
	Weapon
};

class Planet : public Entity {
public:
	Planet(PlanetType type, int windowWidth);
	Planet();
	~Planet();

	PlanetType GetPlanetType();

	//TODO: The implementation of this is limited to 2 types of planets, make sure we change it
	static PlanetType RandomPlanetType(bool hasShotgun);

	static void MovePlanetGroup(std::vector<Planet>* planets, 
		Player* player, const SDL_FRect& extendedWindowR, SDL_bool(*intersects)(const SDL_FRect* a, const SDL_FRect* b));

	virtual SDL_Texture* GetTexture() override;

private:
	PlanetType planetType;
};