#pragma once
#include "Entity.h"
#include "Clock.h"
#include "Bullet.h"

class Enemy : public Entity {
public:
	SpawnPlace spawnPlace = SpawnPlace::Up;

	Enemy(int shootingInterval);

	void Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef);

private:
	Clock shootingClock;
	int shootingInterval;
};