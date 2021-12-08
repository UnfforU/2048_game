#pragma once
#include <Windows.h>
#include "HelpLib.h"

class Block
{
public:
	Point pos;
	double value;
	COLORREF color;

	void SetPosAndColor(int x, int y);

	Block();
	Block(int value, COLORREF color, int posX, int posY);

	~Block();

private:

};

