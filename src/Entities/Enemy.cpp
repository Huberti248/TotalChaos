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

	
	SDL_FPoint targetPos;
	if (this->type == EnemyType::Follower) {
		targetPos = playerPos;
	}
	else {
		MathUtils::Normalize(&enPos);
		//Shoot it straight forward
		switch (this->spawnPlace) {
		case SpawnPlace::Up:
			targetPos = MathUtils::VectorAdd({ enPos.x, 0.0f }, MathUtils::WORLD_DOWN);
			break;
		case SpawnPlace::Down:
			targetPos = MathUtils::VectorAdd({ enPos.x, 0.0f }, MathUtils::WORLD_UP);
			break;
		case SpawnPlace::Left:
			targetPos = MathUtils::VectorAdd({ 0.0f, enPos.y }, MathUtils::WORLD_RIGHT);
			break;
		case SpawnPlace::Right:
			targetPos = MathUtils::VectorAdd({ 0.0f, enPos.y }, MathUtils::WORLD_LEFT);
			break;
		}
	}

	SDL_FPoint finalPos = MathUtils::VectorSubstract(targetPos, enPos);
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

		float movementSpeed = ENEMY_SPEED;

		//If the current enemy is a follower, always direct it to the player
		if (e->type == EnemyType::Follower) {
			e->FollowPlayer();
			movementSpeed *= 1.3f;
		}
		e->r.x += e->dx * deltaTime * movementSpeed;
		e->r.y += e->dy * deltaTime * movementSpeed;
	}
}

void Enemy::HandleInstanceDmg(std::vector<Enemy>* enemies, size_t instanceIndex) {
	bool shouldDie = (*enemies).at(instanceIndex).TakeDamage();
    if (shouldDie) {
		enemies->erase(enemies->begin() + instanceIndex);
		GameManager::playerReference->streak++;
	}
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
	
	this->r.w -= this->originalWidth * 0.1f;
	this->r.h -= this->originalHeight * 0.1f;

	return floor(this->r.w) <= this->originalWidth * 0.7f;
}

void Enemy::FollowPlayer() {
	Player* playerRef = GameManager::playerReference;

	SDL_FPoint playerPos = {
		playerRef->r.x,
		playerRef->r.y
	};

	SDL_FPoint currEnemyPos = {
		this->r.x,
		this->r.y
	};

	//Sub vect (dest - src)
	SDL_FPoint direction = MathUtils::VectorSubstract(playerPos, currEnemyPos);

	MathUtils::Normalize(&direction);

	this->dx = direction.x;
	this->dy = direction.y;
	RotateEntityTowards(this->GetTexture(), *this, playerPos, SdlUtils::renderer);
}

void Enemy::AssignRandomType() {
	this->type = EnemyType::Default; //TODO: Actually randomly assign this
	int prob = MathUtils::Random(0, 100);
	
	if (MathUtils::Between(prob, 0, DEFAULT_ENEMY_PROB)) {
		this->type = EnemyType::Default;
	}
	else if (MathUtils::Between(prob, DEFAULT_ENEMY_PROB + 1, DEFAULT_ENEMY_PROB + TANK_ENEMY_PROB)) {
		this->type = EnemyType::Tank;
		this->r.w = 64;
		this->r.h = 64;
	}
	else {
		this->type = EnemyType::Follower;
		this->shootingInterval /= 3;
	}
}
