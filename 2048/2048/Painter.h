#pragma once
#include <Windows.h>
#include "constants.h"

class Painter
{
public:

	//�������������� ��� ��������, ��������� � �����. �������� ����
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
