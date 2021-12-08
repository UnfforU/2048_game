#pragma once
#include "Block.h"

class Game
{
public:
	Block field[4][4];
	int saveField[4][4];
	int Score;
	int BestScore;
	int ElementsCount;



	Game();

	void StartNewGame(HWND hWnd);
	void RandomizeValueOneTile();

	~Game();

private:

};



