// EATest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;


// helper function for tokenize string into words
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
	string str2 = str;

	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

	string::size_type lastPos = str2.find_first_not_of(delimiters, 0);

	string::size_type pos = str2.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		string word = str2.substr(lastPos, pos - lastPos);

		//erase special characters
		int len = word.size();

		for (int i = len - 1; i >= 0; i--)
		{
			if (!(word[i] >= 'a' && word[i] <= 'z'))
			{
				word.erase(i);
			}
		}

		if (word.size())
		{
			tokens.push_back(word);
		}

		lastPos = str2.find_first_not_of(delimiters, pos);

		pos = str2.find_first_of(delimiters, lastPos);
	}
}


//read files
void readFile(char* filename, map<string, vector<long>>& wordLineMap, long& lineNumber)
{

	ifstream in_stream;

	in_stream.open(filename);

	string line;

	while (!in_stream.eof())
	{

		getline(in_stream, line);

		if (line.size())
		{
			vector<string> tokens;

			Tokenize(line, tokens); // break the sentence into words

			// check each token and map it with line number

			int nToken = tokens.size();

			for (int i = 0; i < nToken; i++)
			{
				if (wordLineMap.find(tokens[i]) != wordLineMap.end())
				{
					wordLineMap[tokens[i]].push_back(lineNumber);

				}
				else
				{
					vector<long> newToken;
					newToken.push_back(lineNumber);
					wordLineMap[tokens[i]] = newToken;

				}

			}
		}

		lineNumber++;

	}

	in_stream.close();


}

// find the intersection of the two arrays
vector<long> FindIntersection(vector<long>& nums1, vector<long>& nums2)
{
	map<long, long> record;
	vector<long> result;

	int len1 = nums1.size();
	int len2 = nums2.size();

	for (int i = 0; i < len1; i++)
	{
		if (record.find(nums1[i]) == record.end())
		{
			record[nums1[i]] = 1;
		}
		else
		{
			record[nums1[i]] ++;
		}
	}

	for (int i = 0; i < len2; i++)
	{
		if (record.find(nums2[i]) != record.end())
		{
			if (record[nums2[i]] != 0)
			{
				result.push_back(nums2[i]);
				record[nums2[i]] --;
			}

		}
	}

	return result;

}



//print matched lines from files
void printLine(char* filename, vector<long>& result, long& GlobalLineNumber, int& resIdx)
{
	ifstream in_stream;

	long localLine = 1;

	int nLen = result.size();

	if (nLen == 0)
	{
		return;
	}

	in_stream.open(filename);

	string line;

	int curlineNumber = result[resIdx];

	while (!in_stream.eof())
	{
		getline(in_stream, line);

		// if the line is to be printed
		if (curlineNumber == GlobalLineNumber)
		{
			cout << filename << endl;
			cout << localLine << endl;
			cout << line << endl;

			resIdx++;
			if (resIdx < nLen)
			{
				curlineNumber = result[resIdx];
			}
			else
			{
				break;
			}
			
		}

		localLine++;
		GlobalLineNumber++;

	}

	in_stream.close();


}




// query sentence
vector<long> QuerySentence(string query, map<string, vector<long>>& wordLineMap)
{
	vector<string> tokens;

	vector<long> result;

	Tokenize(query, tokens);

	int nToken = tokens.size();

	bool record = true;

	for (int i = 0; i < nToken; i++)
	{
		vector<long> tempLineNumer;

		if (wordLineMap.find(tokens[i]) == wordLineMap.end())
		{
			record = false;
			break;
		}
		else if (i==0)
		{
			result = wordLineMap[tokens[i]];
		}
		else
		{
			result = FindIntersection(result, wordLineMap[tokens[i]]); // find the intersection of the line numbers
		}
	}

	if (record)
	{
		return result; 
	}

	result.clear(); //return empty set

	return result;

}


int main(int argc, char* argv[])
{
	map<string, vector<long>> wordLineMap; // the map between word and line number

	int i = 0;

	if (argc == 1)
	{
		cout << "please input file name" << endl;
	}

	int nFiles = argc - 1;

	long GlobalLineNumber = 1; // the global line number across different documents

	while (nFiles)
	{
		char* filename = argv[nFiles];

		readFile(filename, wordLineMap, GlobalLineNumber);

		nFiles--;
	}

	cout << "Processing finished, please enter query sentence" << endl;

	while (true)
	{
		printf("query>");
		string query;

		getline(cin, query);

		vector<long> result;

		if (query != "!exit")
		{
			result = QuerySentence(query, wordLineMap); // query the data set, and return is the line number of corresponding lines

			int nFiles = argc - 1;

			long tempLine = 1; // temp global line indicator

			int resIdx = 0; // result index

			while (nFiles)
			{
				char* filename = argv[nFiles];

				printLine(filename, result ,tempLine, resIdx); // print out match sentences

				nFiles--;
			}

		}
		else
		{
			cout << "Program Exit" << endl;
			return 0;
		}

	}


	return 0;
}
