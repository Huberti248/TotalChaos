#pragma once
#include "Crypto.h"
#include "../Auxiliars/StringUtils.h"
#include "../Entities/Bullet.h"
#include <fstream>
#include <map>
#include "../Entities/Player.h"

#define POWERUP_PATH "res/pwp.dat"

class PowerupManager {
public:
	PowerupManager() {};
	~PowerupManager() {};

	static void InitializePowerUps();
	
	static void SavePowerUps(const std::map<std::string, std::string>& nameAndAttributes, const std::map<std::string, float>& toSave);
	
	static void ReadPowerUps(std::map<std::string, std::string>* nameAndAttributeRef, std::map<std::string, float>* attributeAnValueRef);
	
	static void ReadPowerUps(std::map<std::string, float>* attributeAnValueRef);

private:

	static inline const std::map<std::string, float*> REFLECTION_MAP = {
		{"BulletSpeed", &Bullet::playerBulletSpeed},
		{"MaxHealth", &Player::maxHealth}
	};

	static inline std::string ek = "C1YGbAq4k3NeV77EgwtM";
};