#include <math.h>
#include "Block.h"
#include "HelpLib.h"

Block::Block() {
	this->value = 0;
	this->pos.x = 0;
	this->pos.y = 0;
	this->color = NULL;
}

Block::Block(int value, COLORREF color, int posX, int posY) {
	this->value = value;
	this->color = color;
	this->pos.x = posX;
	this->pos.y = posY;
}

void Block::SetPosAndColor(int x, int y) 
{
	this->pos.x = x;
	this->pos.y = y;

	int curNum = log2(this->value);
	this->color = Colors[curNum - 1];
}

Block::~Block()
{
}