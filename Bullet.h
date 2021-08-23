#pragma once
#include "Entity.h"
#include <vector>

enum class TargetMask {
	Player = 1,
	Enemies
};

class Bullet : public Entity{
public:
	Bullet(TargetMask targetMask);

	void Split(std::vector<Bullet>* bulletListRef);

	TargetMask GetTargetMask();
private:
	TargetMask targetMask;
};
