#include "Enemy.h"

Enemy::Enemy(int shootingInterval) {
	this->shootingInterval = shootingInterval;
	this->shootingClock.restart();
}

void Enemy::Combat(SDL_Texture* enemyTexture, std::vector<Bullet>* bulletListRef) {
	//Check if the interval has passed
	if (this->shootingClock.getElapsedTime() < shootingInterval) return;
	//Rotate towards the player
	//Create a bullet with the appropriate target
	//Shoot it towards the player
}
