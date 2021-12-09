#pragma once
#include "constants.h"
#include "Block.h"

class Game
{
public:
	Block field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	Block historyField[MAX_FIELD_SIZE][MAX_FIELD_SIZE][HISTORY_SIZE];
	long int saveField[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	unsigned long long int score;
	unsigned long long int scoreHistory[HISTORY_SIZE];
	int currHistoryPos = 0;
	unsigned long long int bestScore;
	int elementsCount;
	bool isNewGame;



	Game(HWND hWnd);

	void StartNewGame(HWND hWnd);
	void RandomizeValueOneTile();

	//void SaveIntoHistory();

	bool KeyUpHandler(bool isTest);
	bool KeyDownHandler(bool isTest);
	bool KeyLeftHandler(bool isTest);
	bool KeyRightHandler(bool isTest);

	void SaveIntoHistory();
	void SaveResultsInFile();

	bool isGameOver();

	~Game();

private:

};



