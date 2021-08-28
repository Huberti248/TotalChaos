#pragma once
#include "Entity.h"
#include "Clock.h"
#include "Bullet.h"
#include "Movement.h"
#include "AudioManager.h"

class Enemy : public Entity {
public:
	SpawnPlace spawnPlace = SpawnPlace::Up;

	Enemy(int shootingInterval);

	void Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef, const SDL_FPoint& playerPos, SDL_Renderer* renderer);

private:
	Clock shootingClock;
	int shootingInterval;
	AudioManager* audioManager = AudioManager::Instance();
};