#include "Enemy.h"

Enemy::Enemy(int shootingInterval) {
	this->shootingInterval = shootingInterval;
	this->shootingClock.restart();
	this->r.w = 32;
	this->r.h = 32;
	this->spawnPlace = (SpawnPlace)MathUtils::Random(0, 3);
	AssignRandomType();
	this->originalHeight = this->r.h;
	this->originalWidth = this->r.w;
}

void Enemy::Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef, const SDL_FPoint& playerPos, SDL_Renderer* renderer) {
	//Check if the interval has passed
	if (this->shootingClock.getElapsedTime() < shootingInterval) return;
	
	//Rotate towards the player
	//RotateEntityTowards(enemyTexture, *this, playerPos, renderer);
	audioManager->PlaySFX(SFXAudio::EnemyFire);
	//Create a bullet with the appropriate target
	Bullet toShoot = Bullet(TargetMask::PlayerMask);
	toShoot.r.x = this->r.x;
	toShoot.r.y = this->r.y;

	SDL_FPoint enPos = {
		this->r.x,
		this->r.y
	};

	//Shoot it towards the player
	SDL_FPoint finalPos = MathUtils::VectorSubstract(playerPos, enPos);

	MathUtils::Normalize(&finalPos);

	toShoot.dy = finalPos.y;
	toShoot.dx = finalPos.x;

	//Add the bullet to the vector
	bulletListRef->push_back(toShoot);

	//Restart timer
	this->shootingClock.restart();
}

void Enemy::MoveEnemyGroup(std::vector<Enemy>* enemies) {
	float deltaTime = Clock::GetDeltaTime();
	for (size_t i = 0; i < enemies->size(); i++) {
		Enemy* e = &(enemies->at(i));
		e->r.x += e->dx * deltaTime * ENEMY_SPEED;
		e->r.y += e->dy * deltaTime * ENEMY_SPEED;
	}
}

void Enemy::HandleInstanceDmg(std::vector<Enemy>* enemies, size_t instanceIndex) {
	bool shouldDie = (*enemies).at(instanceIndex).TakeDamage();
	if (shouldDie)
		enemies->erase(enemies->begin() + instanceIndex);
}

SDL_Texture* Enemy::GetTexture() {
	switch (this->type) {
		case EnemyType::Default:
			return TextureLoader::GetTextureByName("Enemy");

		case EnemyType::Tank:
			return TextureLoader::GetTextureByName("TankEnemy");

		case EnemyType::Follower:
			return TextureLoader::GetTextureByName("FollowerEnemy");

		default:
			LOG_LN("Invalid enemy type!");
			return nullptr;
	}
}

bool Enemy::TakeDamage() {
	//Check if it's a proper enemy type
	if (this->type != EnemyType::Tank) return true;
	//Now decrease the entity's size and check if we are below 30%
}

void Enemy::AssignRandomType() {
	this->type = EnemyType::Default; //TODO: Actually randomly assign this
	if (this->type == EnemyType::Tank) {
		this->r.w = 64;
		this->r.h = 64;
	}
}
