#pragma once
#include <Windows.h>
#include "constants.h"
#include "Game.h"

class Painter
{
public:
	Painter();

	//»нициализируем все свойства, свз€анные с окном. «аливаем окно
	void SetHWND(HWND hWnd);
	void Redraw(Game game);
	void PaintOneFrame(Game game);
	void DrawMainElements(int currScore, int bestScore);


	~Painter();

private:
	COLORREF BackgroundColor = RGB(250, 248, 239/*255, 0, 0*/);
	HBRUSH hBackgroundBrush = CreateSolidBrush(BackgroundColor);
	HDC hDC;
	HDC memDC;
	HWND hWnd;
	HBITMAP hBitmap;
	PAINTSTRUCT paintstruct;



	//float TILE_SIZE;
};
