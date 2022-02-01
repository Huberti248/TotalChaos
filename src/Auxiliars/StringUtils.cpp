#include "StringUtils.h"

std::vector<std::string> StringUtils::SplitString(std::string toSplit, char delimitter) {
    std::vector<std::string> result = std::vector<std::string>();
    size_t position = 0;
    while ((position = toSplit.find(delimitter)) != std::string::npos) {
        std::string toPush = toSplit.substr(0, position);
        result.push_back(toPush);
        toSplit.erase(0, position + 1);
    }
    result.push_back(toSplit);
    return result;
}

std::string StringUtils::Tokenize(const std::string& str, char del) {
	std::string temp = "";

	for (size_t i = 0; i < str.size(); i++) {
		// If cur char is not del, then append it to the cur "word", otherwise
		// you have completed the word, print it, and start a new word.
		if (str[i] != del) {
			temp += str[i];
			continue;
		}
		temp = "";
	}
	return temp;
}
