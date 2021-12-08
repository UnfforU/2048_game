#pragma once
#include <Windows.h>
#include "HelpLib.h"

class Block
{
public:
	Point pos;
	int value;
	COLORREF color;

	void SetPos(int i, int j);
	void SetColor();
	//wchar_t* ValueToString();
	Block();
	Block(int value, COLORREF color, int posX, int posY);

	~Block();

private:

};

