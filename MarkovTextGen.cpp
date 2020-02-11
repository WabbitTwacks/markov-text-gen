#include "MarkovTextGen.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <locale>
#include <codecvt>
using namespace std;

bool MarkovTextGen::GenerateDictionaryFromFile(std::string filename)
{
	ifstream fileText;
	fileText.open(filename.c_str());
	
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	
	if (fileText.is_open())
	{
		vector<wstring> vWords;
		string sWord;

		while (fileText >> sWord) //read the file word by word
		{	
			wstring wsWord = converter.from_bytes(sWord);
			vWords.push_back(wsWord);			
		}

		fileText.close();

		bool bEos = true;

		for (auto i = vWords.begin(); i != vWords.end(); i++)
		{
			wstring wsNGram = L"";

			//get an n-gram depending on nGramSize
			if (i + 1 != vWords.end()) //skip if last word
			{
				for (int n = 0; n < nGramSize; n++)
				{
					wsNGram += *(i + n);

					if (n != nGramSize - 1)
						wsNGram += L" ";
				}
			}

			if (bEos) //if last N-Gram was end of sentence, store the current one as a begining of sentence
			{
				vStartingWords.push_back(wsNGram);
				bEos = false;
			}				

			transform(wsNGram.begin(), wsNGram.end(), wsNGram.begin(), ::tolower); //transform to lower-case before adding to the dictionary

			if (i->find_last_of('.') != wstring::npos) //check if end of sentence
				bEos = true;

			if (i + 1 != vWords.end() && !bEos)
				if (i + 2 != vWords.end() && !bEos)
					mDictionary[wsNGram].push_back(*(i + 2));			
		}		
	}
	else
	{
		cout << "Unable to open file: " << filename << endl;
		return false;
	}	

	cout << "MarkovTextGen :: Created dictionary from " << filename << endl;

	return true;
}

std::wstring MarkovTextGen::GenerateSentence()
{
	//get a random begining of sentence
	srand(time(NULL));

	int iRand = rand() % vStartingWords.size();

	wstring sCurNGram = vStartingWords[iRand];
	wstring sSentence = sCurNGram;

	//continue through the dictionary
	wstring sIndex = sCurNGram;	

	while (sCurNGram.find_last_of('.') == string::npos) //until end of sentence
	{
		transform(sIndex.begin(), sIndex.end(), sIndex.begin(), ::tolower);
		vector<wstring> vNext = mDictionary[sIndex];
		int iRandNext = rand() % vNext.size(); //get a random index for the words that follow the current n-gram in the dictionary
		sSentence += L" ";
		sSentence += vNext[iRandNext];
		sCurNGram = sCurNGram.substr(sCurNGram.find_first_of(' ')+1) + L" " + vNext[iRandNext];
		sIndex = sCurNGram;
	}

	return sSentence;
}
