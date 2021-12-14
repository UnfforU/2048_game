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

int GetDeltaX(int rectSize, HDC hDC, LPCWSTR str)
{
	SIZE res;
	GetTextExtentPoint32(hDC, str, wcslen(str), &res);
	return rectSize / 2 - res.cx / 2;
}



string intToStr(int num) {
	static stringstream toStrConverter;
	static string result;
	toStrConverter.clear();
	toStrConverter << num;
	toStrConverter >> result;
	return result;
}

void CreateNewLastSaveFile(int field_size) 
{
	ofstream newFile("Memory\\LastSave\\save_" + intToStr(field_size) + ".dat");
	if(newFile.is_open())
	{
		for (int i = 0; i <= field_size*field_size; i++)
		{
			newFile << "0 ";
		}
		newFile.close();
	}
}

void CreateNewBestScoreFile(int field_size)
{
	ofstream newFile("Memory\\BestScore\\best_" + intToStr(field_size) + ".dat");
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
		CreateNewBestScoreFile(FIELD_SIZE);
		bestScore = 0; 
	}

	return bestScore;
}

void SaveBestScore(int bestScore)
{
	int targetBstScore = GetBestScore();
	ofstream fout("Memory\\BestScore\\best_" + intToStr(FIELD_SIZE) + ".dat", ios_base::out | ios_base::trunc);
	if (fout.is_open())
	{
		if (targetBstScore < bestScore) { targetBstScore = bestScore; }
		string buffStr = intToStr(targetBstScore);
		fout << buffStr;
		
		fout.close();
	}
	else {CreateNewBestScoreFile(FIELD_SIZE); }
}
