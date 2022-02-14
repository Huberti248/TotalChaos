#pragma once
#include "Entity.h"
#include "../Engine/Clock.h"
#include "Bullet.h"
#include "../Mechanics/Movement.h"
#include "../Engine/AudioManager.h"
#include "../Engine/SdlUtils.h"
#include "../Auxiliars/GameManager.h"

//TODO: Replace with variable to use in power ups
#define ENEMY_SPEED 0.1

#pragma region Enemy distributions
//This distribution might change by the permanent power ups(?
#define DEFAULT_ENEMY_PROB 65
#define TANK_ENEMY_PROB 20
#define FOLLOWER_ENEMY_PROB 15
#pragma endregion
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

	void FollowPlayer();

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