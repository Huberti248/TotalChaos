#include "Planet.h"

Planet::Planet(PlanetType type, int windowWidth) {
	this->planetType = type;
	this->r.w = 64;
	this->r.h = 64;
	this->r.x = MathUtils::Random(0, windowWidth - this->r.w);
	this->r.y = -this->r.h;
	this->dy = 1;
}

Planet::Planet() {
	//Just a default planet type for the empty constructor
	this->planetType = PlanetType::Shield;
}

Planet::~Planet() {}

PlanetType Planet::GetPlanetType() {
	return this->planetType;
}

PlanetType Planet::RandomPlanetType(bool hasShotgun) {
	if (!hasShotgun)
		return(PlanetType)MathUtils::Random(0, 1);
	return (PlanetType)MathUtils::Random(0, 0);
}

void Planet::MovePlanetGroup(std::vector<Planet>* planets,
	Player* player, const SDL_FRect& extendedWindowR, SDL_bool(*intersects)(const SDL_FRect* a, const SDL_FRect* b)) {
	float deltaTime = Clock::GetDeltaTime();

	for (size_t i = 0; i < planets->size(); ++i) {
		Planet* p = &(planets->at(i));
		p->r.x += p->dx * deltaTime * PLANET_SPEED;
		p->r.y += p->dy * deltaTime * PLANET_SPEED;
		if (intersects(&(player->r), &(p->r))) {
			if (p->GetPlanetType() == PlanetType::Shield) {
				player->buyingShield = true;
			}
			else if (p->GetPlanetType() == PlanetType::Weapon) {
				player->buyingShotgun = true;
			}
		}
		if (intersects(&(player->r), &(p->r))
			|| !intersects(&(p->r), &extendedWindowR)) {
			planets->erase(planets->begin() + i--);
		}
	}
}

SDL_Texture* Planet::GetTexture() {
	switch (this->planetType) {
		case PlanetType::Shield:
			return TextureLoader::GetTextureByName("ShieldPlanet");
		case PlanetType::Weapon:
			return TextureLoader::GetTextureByName("WeaponPlanet");
		default:
			LOG_CAT_LN("There were no textures found for the planet type: ", (int)this->planetType);
			return nullptr;
	}
}
