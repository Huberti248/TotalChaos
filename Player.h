#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	int health;
	bool ghostBullet;

	Player();

	Player(int health);
};