#pragma once
#include "Entity.h"
#include "../Auxiliars/MathUtils.h"

//TODO: Replace with a static variable to use it with the powerups
#define PLAYER_SPEED 0.3
#define DEFAULT_MAX_HEALTH 15

class Player : public Entity {
public:
	static float maxHealth;

	int shotgunAmmo;
	bool hasBomb;
	bool buyingShield;
	bool buyingShotgun;
	unsigned int streak;

	Player();

	Player(int health);

	int GetHealth();

	void SetHealth(float health);

	void SetHealth(int health);

	void MoveBehaviour(int windowWidth, int windowHeight);

private:
	int health;

};