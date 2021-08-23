#pragma once
#include "Entity.h"
#include <vector>

enum TargetMask {
	PlayerMask = 1,
	EnemiesMask
};

class Bullet : public Entity{
public:
	Bullet(int targetMask);

	void Split(std::vector<Bullet>* bulletListRef);

	TargetMask GetTargetMask();
private:
	TargetMask targetMask;
};
