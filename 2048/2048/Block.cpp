#include <Windows.h>
#include <math.h>
#include <string>
#include "Block.h"
#include "HelpLib.h"
#include "constants.h"

extern int currTilePadding;
extern float TILE_SIZE;
extern int FIELD_SIZE;

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

void Block::SetPos(int i, int j) 
{
	int tmpXPadding = 0;
	int tmpYPadding = 0;
	for (int a = 0; a < j; a++) { tmpXPadding += currTilePadding; }
	for (int a = 0; a < i; a++) { tmpYPadding += currTilePadding; }

	this->pos.x = PLAYINGFIELD_MARGIN + j * TILE_SIZE + currTilePadding + tmpXPadding;
	this->pos.y = WINDOW_MARGIN + i * TILE_SIZE + currTilePadding + tmpYPadding;

}

void Block::SetColor()
{
	int curNum = (log2(this->value) <= 16) ? (log2(this->value)) : 16;
	this->color = Colors[curNum - 1];
}

Block::~Block()
{
}
