#include "Player.h"

Player::Player() {
	this->health = 100;
	this->ghostBullet = false;
	this->streak = 0;
}

Player::Player(int health) {
	this->health = health;
	this->ghostBullet = false;
	this->streak = 0;
}

int Player::GetHealth() {
	return this->health;
}

void Player::SetHealth(int health) {
	//Check if the health is less than the previous one
	if (health < this->health)
		this->streak = 0;
	this->health = health;
}
