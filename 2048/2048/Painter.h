#pragma once
#include <Windows.h>
#include "constants.h"

class Painter
{
public:

	//»нициализируем все свойства, свз€анные с окном. «аливаем окно
	void SetHWND(HWND hWnd);
	

	Painter();
	~Painter();

private:
	HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(250, 248, 239));
	HDC hDC;
	HDC memDC;
	HWND hWnd;
	HBITMAP hBitmap;
};
