#include <Windows.h>
#include <string>
#include "Painter.h"
#include "constants.h"
#include "HelpLib.h"

Painter::Painter()
{
	this->TILE_SIZE = PLAYINGFIELD_SIZE / FIELD_SIZE - 2* TILE_PADDING;
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

void Painter::Redraw()
{
	DrawMainElements(0, 10);
	hDC = BeginPaint(hWnd, &paintstruct);
	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &paintstruct);
	SelectObject(memDC, hBackgroundBrush);
}

void Painter::DrawMainElements(int currScore, int bestScore) 
{
	SelectObject(memDC, hBackgroundBrush);
	Rectangle(memDC, -1, -1, WINDOW_WIDTH, WINDOW_HEIGHT);


	SetBkColor(memDC, RGB(143, 122, 101));
	SetTextColor(memDC, RGB(227, 216, 205));
	wchar_t buffer[100];
	wchar_t scoreTxtBuffer[100];

	HBRUSH hBrush;

#pragma region Прямоугольник с текущим счетом 
	swprintf_s(buffer, L"SCORE");
	swprintf_s(scoreTxtBuffer, L"%d", currScore);
	int txtScoreLenght = wcslen(scoreTxtBuffer);
	hBrush = CreateSolidBrush(RGB(143, 122, 101));
	SelectObject(memDC, hBrush);

	RoundRect(memDC, 20, 10, txtScoreLenght + 120, 70, 15, 13);
	TextOut(memDC, 50, 20, buffer, wcslen(buffer));//title
	TextOut(memDC, 20 + GetCentreXPosByLeng(txtScoreLenght, 100), 40, scoreTxtBuffer, wcslen(scoreTxtBuffer));//сам счет
#pragma endregion

#pragma region Прямоугольник с лучшим счетом
	swprintf_s(buffer, L"BEST");
	swprintf_s(scoreTxtBuffer, L"%d", bestScore);
	txtScoreLenght = wcslen(scoreTxtBuffer);
	hBrush = CreateSolidBrush(RGB(143, 122, 101));
	SelectObject(memDC, hBrush);

	RoundRect(memDC, WINDOW_WIDTH - 140 - txtScoreLenght, 10, WINDOW_WIDTH - 40, 70, 15, 13);
	TextOut(memDC, WINDOW_WIDTH - 105 - txtScoreLenght, 20, buffer, wcslen(buffer));//title
	TextOut(memDC, WINDOW_WIDTH - 140 - txtScoreLenght + GetCentreXPosByLeng(txtScoreLenght, 100), 40, scoreTxtBuffer, wcslen(scoreTxtBuffer));//сам счет
#pragma endregion


	hBrush = CreateSolidBrush(RGB(187, 173, 160));
	SelectObject(memDC, hBrush);
	
	//Внешний квадрат (темносерый)
	RoundRect
	(
		memDC, 
		PLAYINGFIELD_MARGIN, //x1 верхний левый
		WINDOW_MARGIN, //y1 верхний левый
		PLAYINGFIELD_SIZE, //x2 нижний правый
		WINDOW_MARGIN + PLAYINGFIELD_SIZE - PLAYINGFIELD_MARGIN, //y2 нижнмй правый
		15, //x3 ширина скругения
		13 //y3 высота
	);

	hBrush = CreateSolidBrush(RGB(205, 193, 180));
	SelectObject(memDC, hBrush);
	int tmpXPadding = 0;
	int tmpYPadding = 0;
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			//Внутренние квадраты(светлосерые
			RoundRect
			(
				memDC, 
				PLAYINGFIELD_MARGIN + j * TILE_SIZE + TILE_PADDING + tmpXPadding,
				WINDOW_MARGIN + i * TILE_SIZE + TILE_PADDING + tmpYPadding, 
				PLAYINGFIELD_MARGIN + j * TILE_SIZE + TILE_PADDING + TILE_SIZE + tmpXPadding,
				WINDOW_MARGIN + i * TILE_SIZE + TILE_PADDING + TILE_SIZE + tmpYPadding,
				15, 
				13);
			tmpXPadding += TILE_PADDING;
		}
		tmpXPadding = 0;
		tmpYPadding += TILE_PADDING;
	}

	DeleteObject(hBrush);
}

Painter::~Painter()
{
	DeleteObject(Painter::hBackgroundBrush);
	DeleteDC(Painter::memDC);
}