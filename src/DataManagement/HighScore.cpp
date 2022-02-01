#include "HighScore.h"

void HighScores::WriteHighScore(const std::tuple<int, std::string> scores[HIGH_SCORES_LIMIT]) {
	//Open the file
	std::ofstream file(HIGH_SCORES_PATH);
	//Iterate a defined number of times (HIGH_SCORES_LIMIT)
	//Rewrite the file
	for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
		std::string name = std::get<1>(scores[i]);
		int score = std::get<0>(scores[i]);
		if (score < 0) continue;
		//Encrypt
		std::string toWrite =  Crypto::Encrypt(std::to_string(score) + " " + name, ek);
		file << toWrite << std::endl;
	}
	file.close();
}

void HighScores::ReadScores(std::tuple<int, std::string> scorePairsRef[HIGH_SCORES_LIMIT]) {
	std::ifstream file(HIGH_SCORES_PATH);
	std::string buffer;
	//Iterate through the file HIGH_SCORES_LIMIT times
	for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
		if (std::getline(file, buffer)) {
			//Decrypt
			std::string decryptedBuffer = Crypto::Decrypt(buffer, ek);
			std::string name = StringUtils::Tokenize(decryptedBuffer, ' ');
			int score = std::stoi(decryptedBuffer.substr(0, decryptedBuffer.length() - name.length()));
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
