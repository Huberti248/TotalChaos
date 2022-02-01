#include "PowerupManager.h"

void PowerupManager::InitializePowerUps() {
	//Go and read from the file all of our current power ups
	std::map<std::string, float> testPowerUpAttributesAndValues;
	
	ReadPowerUps(&testPowerUpAttributesAndValues);
	//Call all modules that are affected by powerups, and send them a map of
	//the attribute that we want to change, and what we want to change it to.

	for (const auto& [key, value] : testPowerUpAttributesAndValues) {
		(*REFLECTION_MAP.at(key)) = value;
	}
}

void PowerupManager::SavePowerUps(const std::map<std::string, std::string>& nameAndAttributes, const std::map<std::string, float>& toSave) {
	std::ofstream powerUpsFile;
	powerUpsFile.open(POWERUP_PATH, std::ios::app);

	std::string lineBuffer;

	for (const auto& [key, value] : nameAndAttributes) {
		lineBuffer = key + "," + value + "," + std::to_string(toSave.at(value));
		lineBuffer = Crypto::Encrypt(lineBuffer, ek);
		powerUpsFile << lineBuffer << std::endl;
	}
}

void PowerupManager::ReadPowerUps(std::map<std::string, std::string>* nameAndAttributeRef, std::map<std::string, float>* attributeAnValueRef) {
	std::ifstream powerUpsFile;
	powerUpsFile.open(POWERUP_PATH);

	std::string buffer;

	while (std::getline(powerUpsFile, buffer)) {
		//This is the decrypted part
		std::string decrypted = Crypto::Decrypt(buffer, ek);
		//The decrypted line will look like this: NamePowerUp,NameOfAttribute,Value
		auto lineParts = StringUtils::SplitString(decrypted, ',');
		(*nameAndAttributeRef)[lineParts[0]] = lineParts[1];
		(*attributeAnValueRef)[lineParts[1]] = (float)atof(lineParts[2].c_str());
	}
}

void PowerupManager::ReadPowerUps(std::map<std::string, float>* attributeAnValueRef) {
	std::ifstream powerUpsFile;
	powerUpsFile.open(POWERUP_PATH);

	std::string buffer;

	while (std::getline(powerUpsFile, buffer)) {
		//This is the decrypted part
		std::string decrypted = Crypto::Decrypt(buffer, ek);
		//The decrypted line will look like this: NamePowerUp,NameOfAttribute,Value
		auto lineParts = StringUtils::SplitString(decrypted, ',');
		(*attributeAnValueRef)[lineParts[1]] = (float)atof(lineParts[2].c_str());
	}
}
