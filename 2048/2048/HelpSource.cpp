#include <Windows.h>
#include <sstream>
#include <fstream>
#include <string>
#include "HelpLib.h"


using namespace std;

extern int currTilePadding;
extern float TILE_SIZE;
extern int FIELD_SIZE;


int GetCentreXPosByLeng(int length, int blockSize)
{
	return blockSize/2 - length * (length / 2);
}

int GetDeltaX(int num) {
	if (num > 1000)
		return TILE_SIZE / 2 - 5;
	if (num > 100)
		return TILE_SIZE / 2 - 3;
	if (num > 10)
		return TILE_SIZE / 2 - 12;
	return TILE_SIZE / 2 - 5;
}

string intToStr(int num) {
	static stringstream toStrConverter;
	static string result;
	toStrConverter.clear();
	toStrConverter << num;
	toStrConverter >> result;
	return result;
}

void CreateNewLastSaveFile() 
{
	ofstream newFile("Memory\\LastSave\\save_" + intToStr(FIELD_SIZE) + ".dat");
	if(newFile.is_open())
	{
		for (int i = 0; i <= FIELD_SIZE*FIELD_SIZE; i++)
		{
			newFile << "0 ";
		}
		newFile.close();
	}
}

void CreateNewBestScoreFile()
{
	ofstream newFile("Memory\\BestScore\\best_" + intToStr(FIELD_SIZE) + ".dat");
	if (newFile.is_open())
	{
		newFile << "0";
		newFile.close();
	}
}

int GetBestScore()
{
	string buffStr;
	int bestScore;
	ifstream fin("Memory\\BestScore\\best_" + intToStr(FIELD_SIZE) + ".dat");
	if (fin.is_open())
	{
		fin >> buffStr;
		buffStr = (buffStr == "") ? "0" : buffStr;
		bestScore = stoi(buffStr);
		fin.close();
	}
	else 
	{
		CreateNewBestScoreFile();
		bestScore = 0; 
	}

	return bestScore;
}

void SaveBestScore(int bestScore)
{
	ofstream fout("Memory\\BestScore\\best_" + intToStr(FIELD_SIZE) + ".dat", ios_base::out | ios_base::trunc);
	if (fout.is_open())
	{
		string buffStr = intToStr(bestScore);
		fout << buffStr;
		fout.close();
	}
	else {CreateNewBestScoreFile(); }
}