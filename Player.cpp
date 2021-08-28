#include "Player.h"
//#include <cstdio>

Player::Player() {
	this->health = 100;
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

void Player::SetHealth(int health) {
	//Check if the health is less than the previous one
	if (health < this->health) {
		//printf("Lost streak of %u\n", this->streak);
		this->streak = 0;
	}
	this->health = health;
}
