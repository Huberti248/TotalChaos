#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	int health;

	Player();

	Player(int health);
};