#include "Player.h"

float Player::maxHealth = DEFAULT_MAX_HEALTH;

Player::Player() {
	this->health = 50;
	this->shotgunAmmo = 0;
	this->streak = 0;
	this->hasBomb = false;
}

Player::Player(int health) {
	this->health = health;
	this->shotgunAmmo = 0;
	this->streak = 0;
	this->hasBomb = false;
}

int Player::GetHealth() {
	return this->health;
}

void Player::SetHealth(float health) {
	//Round up or down
	int roundedHealth = MathUtils::RoundToClosestInteger(health);
	this->SetHealth(roundedHealth);
}

void Player::SetHealth(int health) {
	//Check if the health is less than the previous one
	if (health < this->health) {
		//printf("Lost streak of %u\n", this->streak);
		this->streak = 0;
	}
	this->health = health;
}

void Player::MoveBehaviour(int windowWidth, int windowHeight) {
	//Player movement
	float deltaTime = Clock::GetDeltaTime();
	this->r.x += this->dx * deltaTime * PLAYER_SPEED;
	this->r.y += this->dy * deltaTime * PLAYER_SPEED;
	this->r.x = MathUtils::Clamp(this->r.x, 0.0f, windowWidth - this->r.w);
	this->r.y = MathUtils::Clamp(this->r.y, 0.0f, windowHeight - this->r.h);
}
