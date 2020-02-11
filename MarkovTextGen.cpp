#include "MarkovTextGen.h"

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

bool MarkovTextGen::GenerateFromFile(std::string filename)
{
	ifstream fileText;
	fileText.open(filename.c_str());
	
	if (fileText.is_open())
	{
		vector<string> vWords;
		string sWord;

		while (fileText >> sWord) //read the file word by word
		{	
			vWords.push_back(sWord);			
		}

		fileText.close();

		bool bEos = true;

		for (auto i = vWords.begin(); i != vWords.end(); i++)
		{
			string sNGram = "";

			//get an n-gram depending on nGramSize
			if (i + 1 != vWords.end()) //skip if last word
			{
				for (int n = 0; n < nGramSize; n++)
				{
					sNGram += *(i + n);

					if (n != nGramSize - 1)
						sNGram += " ";
				}
			}

			if (bEos) //if last N-Gram was end of sentence, store the current one as a begining of sentence
			{
				vStartingWords.push_back(sNGram);
				bEos = false;
			}				

			transform(sNGram.begin(), sNGram.end(), sNGram.begin(), ::tolower); //transform to lower-case before adding to the dictionary

			if (i + 1 != vWords.end())
				if (i + 2 != vWords.end())
					mDictionary[sNGram].push_back(*(i + 2));

			if (i->find('.')) //check if end of sentence
				bEos = true;
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
