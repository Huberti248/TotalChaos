#pragma once
#include "Entity.h"
#include "../Engine/Clock.h"
#include <vector>

#define BULLET_SHIELD_BOUNCE_TOLERANCE 200
#define DEFAULT_BULLET_SPEED 0.5f

enum TargetMask {
	PlayerMask = 1,
	EnemiesMask
};

class Bullet : public Entity{
public:

	static float sizeDecreaseRatePlayer;
	static float sizeDecreaseRateEnemy;
	
	static float playerBulletSpeed;
	static float enemyBulletSpeed;
	
	Clock shieldBounceDelay;

	float lifetime;
	bool bouncedOffShield;

	Bullet(int targetMask);

	void Split(std::vector<Bullet>* bulletListRef);

	TargetMask GetTargetMask();

	bool DecreaseSize();

	float GetBulletSpeed();

	static void MoveBulletGroup(std::vector<Bullet>* bullets);

	virtual SDL_Texture* GetTexture() override;

private:
	
	TargetMask targetMask;
	float originalWidth;
	float originalHeight;
};
