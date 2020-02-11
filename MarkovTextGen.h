#pragma once

#include <string>
#include <map>
#include <vector>

class MarkovTextGen
{
public:
	bool GenerateDictionaryFromFile(std::string filename);
	std::wstring GenerateSentence();

private:
	std::map<std::wstring, std::vector<std::wstring>> mDictionary;
	uint8_t nGramSize = 2;
	std::vector<std::wstring> vStartingWords;
};

