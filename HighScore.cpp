#include "HighScore.h"

void HighScores::WriteHighScore(std::tuple<int, const char*> scoreNamePair, const std::tuple<int, const char*>*& scores) {
	//Open the file
	std::ofstream file(HIGH_SCORES_PATH);
	//Iterate a defined number of times (HIGH_SCORES_LIMIT)
	//Rewrite the file
	for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
		//Check if the passed score is bigger than the current one, and if it is, write the scoreNamePair instead
		// of the indexed scores tuple
		file << std::get<1>(scoreNamePair) << " " << std::get<0>(scoreNamePair) << std::endl; //Case for the scoreNamePair being Higher
		
	}
	file.close();
}

void HighScores::ReadScores(std::tuple<int, std::string> scorePairsRef[HIGH_SCORES_LIMIT]) {
	std::ifstream file(HIGH_SCORES_PATH);
	std::string buffer;
	//Iterate through the file HIGH_SCORES_LIMIT times
	for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
		if (std::getline(file, buffer)) {
			std::string name = Split(buffer, ' ');
			int score = std::stoi(buffer.substr(0, buffer.length() - name.length()));
			//There's content, so we place the scores in the tuple
			scorePairsRef[i] = std::make_tuple(score, name);
		}
		else {
			//Just set the index to an empty tuple
			scorePairsRef[i] = std::make_tuple(-1, "");
		}
	}
	file.close();
}

std::string HighScores::Split(const std::string& str, char del) {
	std::string temp = "";

	for (int i = 0; i < (int)str.size(); i++) {
		// If cur char is not del, then append it to the cur "word", otherwise
		// you have completed the word, print it, and start a new word.
		if (str[i] != del) {
			temp += str[i];
		}
		else {
			temp = "";
		}
	}
	return temp;
}
