#include <Windows.h>
#include "Painter.h"

Painter::Painter()
{
}

void Painter::SetHWND(HWND hWnd) 
{
	this->hDC = GetDC(hWnd);
	this->memDC = CreateCompatibleDC(hDC);
	this->hWnd = hWnd;
	this->hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(memDC, hBitmap);
	ReleaseDC(hWnd, hDC);
}

Painter::~Painter()
{
	DeleteObject(Painter::hBackgroundBrush);
	DeleteDC(Painter::memDC);
}