#include <Windows.h>
#include "HelpLib.h"

extern int currTilePadding;
extern float TILE_SIZE;
extern int FIELD_SIZE;


int GetCentreXPosByLeng(int length, int blockSize)
{
	return blockSize/2 - length * (length / 2);
}

int GetDeltaX(int num) {
	if (num > 1000)
		return TILE_SIZE / 2 - 5;
	if (num > 100)
		return TILE_SIZE / 2 - 3;
	if (num > 10)
		return TILE_SIZE / 2 - 12;
	return TILE_SIZE / 2 - 5;
}

