#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	SpawnPlace spawnPlace = SpawnPlace::Up;
};