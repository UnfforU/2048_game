#include <Windows.h>
#include "HelpLib.h"

int GetCentreXPosByLeng(int length, int blockSize)
{
	return blockSize/2 - length * (length / 2);
}