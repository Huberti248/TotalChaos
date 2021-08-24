#pragma once
#include "Entity.h"
#include "Clock.h"
#include <vector>

#define BULLET_SHIELD_BOUNCE_TOLERANCE 200

enum TargetMask {
	PlayerMask = 1,
	EnemiesMask
};

class Bullet : public Entity{
public:
	Clock shieldBounceDelay;

	float lifetime;
	bool bouncedOffShield;

	Bullet(int targetMask);

	void Split(std::vector<Bullet>* bulletListRef);

	TargetMask GetTargetMask();
private:
	TargetMask targetMask;
};
