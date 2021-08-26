#include "Player.h"

Player::Player() {
	this->health = 100;
	this->ghostBullet = false;
}

Player::Player(int health) {
	this->health = health;
	this->ghostBullet = false;
}
