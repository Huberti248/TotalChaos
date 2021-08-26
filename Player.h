#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	bool ghostBullet;
	unsigned int streak;

	Player();

	Player(int health);

	int GetHealth();

	void SetHealth(int health);

private:
	int health;

};