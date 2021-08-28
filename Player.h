#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	int shotgunAmmo;
	bool hasBomb;
	unsigned int streak;

	Player();

	Player(int health);

	int GetHealth();

	void SetHealth(int health);

private:
	int health;

};