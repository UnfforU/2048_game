#pragma once
#include <Windows.h>
#include "HelpLib.h"

class Block
{
public:
	Point pos;
	long int value;
	COLORREF color;
	bool isUnitable = true;

	void SetPos(int i, int j);
	void SetColor();
	
	Block();

	Block(int value, COLORREF color, int posX, int posY);

	~Block();

private:

};

