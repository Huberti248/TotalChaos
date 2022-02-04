#include "LevelingManager.h"

void LevelingManager::ReadAndInitializeExperience() {
	std::ifstream file;
	file.open(EXPERIENCE_PATH);
	std::string buffer;
	
	if (!std::getline(file, buffer)) {
		file.close();
		return;
	}
	std::vector<std::string> parts = StringUtils::SplitString(buffer, ',');
	//Part 0 is the XP, and 1 is the SP
	LevelingManager::currentXP = atof(parts[0].c_str());
	LevelingManager::currentSP = atoi(parts[1].c_str());
	file.close();
}

void LevelingManager::SaveExperience() {
	std::ofstream file;
	file.open(EXPERIENCE_PATH);
	file << LevelingManager::currentXP << "," << LevelingManager::currentSP;
	file.close();
}
