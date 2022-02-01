#pragma once
#include "Entity.h"
#include "../Engine/Clock.h"
#include "Bullet.h"
#include "../Mechanics/Movement.h"
#include "../Engine/AudioManager.h"

//TODO: Replace with variable to use in power ups
#define ENEMY_SPEED 0.1

class Enemy : public Entity {
public:
	SpawnPlace spawnPlace = SpawnPlace::Up;

	Enemy(int shootingInterval);

	void Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef, const SDL_FPoint& playerPos, SDL_Renderer* renderer);

	static void MoveEnemyGroup(std::vector<Enemy>* enemies);


private:
	Clock shootingClock;
	int shootingInterval;
	AudioManager* audioManager = AudioManager::Instance();
};