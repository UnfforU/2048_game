#pragma once
#include "constants.h"
#include "Block.h"

class Game
{
public:
	Block field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	int saveField[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
	long Score;
	long BestScore;
	int ElementsCount;



	Game();

	void StartNewGame(HWND hWnd);
	void RandomizeValueOneTile();

	void SaveIntoHistory();

	void KeyUpHandler();
	void KeyDownHandler();
	void KeyLeftHandler();
	void KeyRightHandler();

	bool isGameOver();

	~Game();

private:

};



