#pragma once
#include <fstream>
#include "Crypto.h"
#include "../Auxiliars/StringUtils.h"
#define EXPERIENCE_PATH "res/xp.dat"
/*
 * Leveling up will not be managed as, the player is now lvl 69 and that makes you better in some way, no
 * once you get a new level, you get a skill point that you can later spend in permanent power ups such
 * as more health, increased bullet speed, increased bullet resistance, higher chances of a certain temporary pwp, 
 * etc, etc. For more info on available power ups, go to PowerUpManager.h
*/

class LevelingManager {
public:
	static float currentXP;
	static int currentSP;
	
	LevelingManager() {};
	~LevelingManager() {};

	static void ReadAndInitializeExperience();

	static void SaveExperience();
private:
	static inline std::string ek = "XwOc5KBFezhzViaoVWhdHPWT2";
};