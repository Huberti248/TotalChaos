#pragma once
#include <string>
#include <vector>

class StringUtils {
public:
	StringUtils() {};
	~StringUtils() {};

	static std::vector<std::string> SplitString(std::string toSplit, char delimitter);
	
	/// <summary>
	/// WARNING, THIS IS HEAP ALLOCATED!!!
	/// </summary>
	/// <param name="str">Reference to the string to split</param>
	/// <param name="del">Delimitter on which to split the string</param>
	/// <returns>A pointer to a heap allocated string</returns>
	static std::string Tokenize(const std::string& str, char del);
};
