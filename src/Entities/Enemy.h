#pragma once
#include "Entity.h"
#include "../Engine/Clock.h"
#include "Bullet.h"
#include "../Mechanics/Movement.h"
#include "../Engine/AudioManager.h"

//TODO: Replace with variable to use in power ups
#define ENEMY_SPEED 0.1

//TODO: Create enemy type enum instead of new classes

enum class EnemyType {
	Default,
	Tank,
	Follower
};

class Enemy : public Entity {
public:
	SpawnPlace spawnPlace = SpawnPlace::Up;

	Enemy(int shootingInterval);

	void Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef, const SDL_FPoint& playerPos, SDL_Renderer* renderer);


	virtual SDL_Texture* GetTexture() override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns>True if the enemy should be killed</returns>
	bool TakeDamage();

	static void MoveEnemyGroup(std::vector<Enemy>* enemies);

	/// <summary>
	/// Applies damage to the enemy, and removes it from the vector if applicable
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="instanceIndex"></param>
	static void HandleInstanceDmg(std::vector<Enemy>* enemies, size_t instanceIndex);

private:
	Clock shootingClock;
	EnemyType type;
	AudioManager* audioManager = AudioManager::Instance();
	int shootingInterval;
	float originalWidth;
	float originalHeight;

	void AssignRandomType();
};