#pragma once
#include <Windows.h>

struct Point
{
	int x, y;
};

 static COLORREF Colors[17] = {
        RGB(238, 228, 218), //2
        RGB(237, 224, 200), //4
        RGB(242, 177, 121), //8
        RGB(245, 149, 99),  //16
        RGB(243, 125, 95),  //32
        RGB(254, 92, 65),   //64
        RGB(245, 208, 119), //128
        RGB(249, 208, 103), //256
        RGB(249, 202, 88),  //512
        RGB(239, 196, 65),  //1024
        RGB(244, 198, 42),  //2048
        RGB(239, 103, 112), //4096
        RGB(236, 77, 89),   //8192
        RGB(226, 68, 58),   //16384
        RGB(115, 180, 215), //32768
        RGB(95, 160, 223),  //65536
        RGB(3, 124, 190)    //131072
};

 //Различное смещение при разном размере поля(получить нужное - обратиться к элементу массива: размер поля - 3)
 static int TILE_PADDING[4] = {
     10,
     7,
     5,
     4
 };

int GetCentreXPosByLeng(int length, int blockSize);
int GetDeltaX(int num);