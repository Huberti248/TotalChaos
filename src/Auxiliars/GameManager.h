#pragma once
#include "../Entities/Player.h"

class GameManager {
public:
	static inline Player* playerReference = nullptr;

	GameManager() {};
	~GameManager() {};
};