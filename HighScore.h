#pragma once
#include "Crypto.h"
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
	/// <summary>
	/// WARNING, THIS IS HEAP ALLOCATED!!!
	/// </summary>
	/// <param name="str">Reference to the string to split</param>
	/// <param name="del">Delimitter on which to split the string</param>
	/// <returns>A pointer to a heap allocated string</returns>
	static std::string Split(const std::string& str, char del);

	static inline std::string ek = "q1YZbqq48XNeV77Egwtk";
};