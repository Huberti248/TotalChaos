#include "HighScore.h"

void HighScores::WriteHighScore(std::tuple<int, const char*> scoreNamePair, const std::tuple<int, const char*>*& scores) {
	//Open the file
	std::ofstream file(HIGH_SCORES_PATH);
	//Iterate a defined number of times (HIGH_SCORES_LIMIT)
	for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
		//Check if the passed score is bigger than the current one, and if it is, write the scoreNamePair instead
		// of the indexed scores tuple
		file << std::get<1>(scoreNamePair) << " " << std::get<0>(scoreNamePair) << std::endl; //Case for the scoreNamePair being Higher
		
	}
	//Append to the file
	file.close();
}

void HighScores::ReadScores(std::tuple<int, const char*>** scorePairsRef) {
	std::ifstream file(HIGH_SCORES_PATH);
	//Iterate through the file
	//Place the scores as a tuple in the scorePairsRef
}
