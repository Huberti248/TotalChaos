#include "Bullet.h"

float Bullet::sizeDecreaseRatePlayer = 0.05f;
float Bullet::sizeDecreaseRateEnemy = 0.05f;
float Bullet::enemyBulletSpeed = DEFAULT_BULLET_SPEED;
float Bullet::playerBulletSpeed = DEFAULT_BULLET_SPEED;

Bullet::Bullet(int targetMask) {
	this->targetMask = (TargetMask)targetMask;
	this->lifetime = 0.0f;
	this->bouncedOffShield = false;
	this->r.h = 29;
	this->r.w = 29;
	this->originalHeight = r.h;
	this->originalWidth = r.w;
}

void Bullet::Split(std::vector<Bullet>* bulletListRef) {
	//Create a new bullet and add it to the bullet's list
	//Change the current bullet's layerMask to be targetting both enemies and player targetMask = (Player | Enemies);
	//In case the bullet was fired by an enemy, keep the player mask as the player's one
	int targetMask = this->targetMask == 1 ? TargetMask::PlayerMask : TargetMask::PlayerMask | TargetMask::EnemiesMask;
	Bullet splitBullet = Bullet(targetMask);
	splitBullet.r = this->r;
	splitBullet.r.w = this->originalWidth;
	splitBullet.r.h = this->originalHeight;

	//Determine the direction by going the opposite of the current direction and offset it by +- 45°
	float angle = 45 * M_PI / 180.0f;
	splitBullet.dx = -this->dx * cosf(angle) + this->dy * sinf(angle);
	splitBullet.dy = -this->dx * sinf(angle) - this->dy * cosf(angle);

	bulletListRef->push_back(splitBullet);
}

TargetMask Bullet::GetTargetMask() {
	return this->targetMask;
}

//Returns wether or not the bullet should be destroyed
bool Bullet::DecreaseSize() {
	float decreaseRate;
	if (this->targetMask == (TargetMask::PlayerMask | TargetMask::EnemiesMask) || this->targetMask == TargetMask::EnemiesMask) {
		decreaseRate = sizeDecreaseRatePlayer;
	}
	else {
		decreaseRate = sizeDecreaseRateEnemy;
	}

	this->r.h -= this->originalHeight * decreaseRate;
	this->r.w -= this->originalWidth * decreaseRate;
	//If the bullet gets to 30% of its size, dissappear
	return this->r.h <= this->originalHeight * 0.7f;
}

float Bullet::GetBulletSpeed() {
	return this->targetMask == TargetMask::EnemiesMask ? Bullet::playerBulletSpeed : Bullet::enemyBulletSpeed;
}

void Bullet::MoveBulletGroup(std::vector<Bullet>* bullets) {
	float deltaTime = Clock::GetDeltaTime();
	for (size_t i = 0; i < bullets->size(); i++) {
		Bullet* b = &(bullets->at(i));
		b->r.y += b->dy * deltaTime * b->GetBulletSpeed();
		b->r.x += b->dx * deltaTime * b->GetBulletSpeed();
	}
}
