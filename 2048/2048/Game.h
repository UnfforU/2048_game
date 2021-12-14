#pragma once
#include "constants.h"
#include "Block.h"

class Game
{
public:
	Block field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	Block historyField[MAX_FIELD_SIZE][MAX_FIELD_SIZE][HISTORY_SIZE];
	long int saveField[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	long long int score;
	long long int scoreHistory[HISTORY_SIZE];
	int currHistoryPos = 0;
	long long int bestScore;
	int elementsCount;
	bool isNewGame;
	bool isGodMode;


	Game(HWND hWnd);

	void StartNewGame(HWND hWnd);
	void RandomizeValueOneTile();

	bool KeyUpHandler(bool isTest);
	bool KeyDownHandler(bool isTest);
	bool KeyLeftHandler(bool isTest);
	bool KeyRightHandler(bool isTest);

	void SaveIntoHistory();
	void SetLastHistoryToField();
	void SaveResultsInFile();

	int IsGameOver();

	~Game();

private:

};



