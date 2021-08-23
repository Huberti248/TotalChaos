#pragma once
#include "Entity.h"
#include <vector>

class Bullet : public Entity{
public:
	void Split(std::vector<Bullet>* bulletListRef);
};
