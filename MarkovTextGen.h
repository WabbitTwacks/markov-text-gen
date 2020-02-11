#pragma once

#include <string>
#include <map>
#include <vector>

class MarkovTextGen
{
public:
	bool GenerateFromFile(std::string filename);

private:
	std::map<std::string, std::vector<std::string>> mDictionary;
	uint8_t nGramSize = 2;
	std::vector<std::string> vStartingWords;
};

