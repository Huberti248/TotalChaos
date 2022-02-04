#pragma once
#include "Entity.h"
#include "../Auxiliars/MathUtils.h"
#include "../Engine/WindowManager.h"

//TODO: Replace with a static variable to use it with the powerups
#define PLAYER_SPEED 0.3
#define DEFAULT_MAX_HEALTH 15

class Player : public Entity {
public:
	static float maxHealth;

	int shotgunAmmo;
	bool buyingShield;
	bool buyingShotgun;
	bool hasBomb;
	bool hasShield;
	bool hasShotgun;
	unsigned int streak;

	Player();

	void ResetPlayer();

	int GetHealth();

	void SetHealth(float health);

	void SetHealth(int health);

	void MoveBehaviour(int windowWidth, int windowHeight);

	virtual SDL_Texture* GetTexture() override;

private:
	int health;

};