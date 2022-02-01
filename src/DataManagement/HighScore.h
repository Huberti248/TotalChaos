#pragma once
#include "Crypto.h"
#include "../Auxiliars/StringUtils.h"
#include <tuple>
#include <fstream>

#define HIGH_SCORES_PATH "res/hsc.dat"
#define HIGH_SCORES_LIMIT 3

class HighScores {
public:
	HighScores() {}
	~HighScores() {}

	static void HighScores::WriteHighScore(const std::tuple<int, std::string> scores[HIGH_SCORES_LIMIT]);

	static void HighScores::ReadScores(std::tuple<int, std::string> scorePairsRef[HIGH_SCORES_LIMIT]);

private:
	static inline std::string ek = "q1YZbqq48XNeV77Egwtk";
};