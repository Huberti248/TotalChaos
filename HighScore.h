#pragma once
#include "Crypto.h"
#include <tuple>
#include <fstream>

#define HIGH_SCORES_PATH "res/hsc.dat"
#define HIGH_SCORES_LIMIT 5

class HighScores {
public:
	HighScores() {}
	~HighScores() {}

	void WriteHighScore(std::tuple<int, const char*> scoreNamePair, const std::tuple<int, const char*>*& scores);

	static void ReadScores(std::tuple<int, const char*>** scorePairsRef);

private:
	static std::string Split(const std::string& str, char del);
};