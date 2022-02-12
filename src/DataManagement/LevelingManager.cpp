#include "LevelingManager.h"

float LevelingManager::currentXP;
int LevelingManager::currentSP;

void LevelingManager::ReadAndInitializeExperience() {
	std::ifstream file;
	file.open(EXPERIENCE_PATH);
	std::string buffer;
	
	if (!std::getline(file, buffer)) {
		file.close();
		return;
	}
	std::vector<std::string> parts = StringUtils::SplitString(Crypto::Decrypt(buffer, ek), ',');
	//Part 0 is the XP, and 1 is the SP
	LevelingManager::currentXP = atof(parts[0].c_str());
	LevelingManager::currentSP = atoi(parts[1].c_str());
	file.close();
}

void LevelingManager::SaveExperience() {
	std::ofstream file;
	file.open(EXPERIENCE_PATH);
	std::string toWrite = std::to_string(currentXP) + "," + std::to_string(currentSP);
	file << Crypto::Encrypt(toWrite, ek);
	file.close();
}

float LevelingManager::GetXP() {
	return LevelingManager::currentXP;
}

int LevelingManager::GetSP() {
	return LevelingManager::currentSP;
}

void LevelingManager::IncreaseXP(int killStreak) {
	//f(x) = 
}
