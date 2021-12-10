#include <Windows.h>
#include <string>
#include "Painter.h"
#include "constants.h"
#include "HelpLib.h"

extern int currTilePadding;
extern float TILE_SIZE;
extern int FIELD_SIZE;

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

void Painter::Redraw(Game game)
{
	DrawMainElements(game.score, game.bestScore);
	

	hDC = BeginPaint(hWnd, &paintstruct);
	
	PaintOneFrame(game);
	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);
	
	EndPaint(hWnd, &paintstruct);
	SelectObject(memDC, hBackgroundBrush);
}

void Painter::PaintOneFrame(Game game)
{
	wchar_t buff[100];
	int fontHeight;
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			if (game.field[i][j].value != 0) {
				HBRUSH hBlockBrush = CreateSolidBrush(game.field[i][j].color);
				fontHeight = FONT_SIZES[FIELD_SIZE - 3];

				SelectObject(memDC, hBlockBrush);
				RoundRect(memDC, game.field[i][j].pos.x, game.field[i][j].pos.y, game.field[i][j].pos.x + TILE_SIZE, game.field[i][j].pos.y + TILE_SIZE, 15, 13);
				//SetBkColor(memDC, game.field[i][j].color);
				SetBkMode(memDC, TRANSPARENT);


				if ((game.field[i][j].value == 2) || (game.field[i][j].value == 4)) { SetTextColor(memDC, RGB(119, 110, 101)); }
				else { SetTextColor(memDC, RGB(255, 255, 255)); }

				HFONT oldFont, newFont;
				newFont = CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
				oldFont = (HFONT)SelectObject(memDC, newFont);

				swprintf_s(buff, L"%d\0", game.field[i][j].value);

				//динамический размер шрифта для больших чисел
				SIZE res;
				GetTextExtentPoint32(memDC, buff, wcslen(buff), &res);
				while (res.cx > TILE_SIZE) {
					fontHeight--;
					newFont = CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
					SelectObject(memDC, newFont);
					GetTextExtentPoint32(memDC, buff, wcslen(buff), &res);
				}

				int delX = GetDeltaX(TILE_SIZE, memDC, buff);
				int delY = TILE_SIZE / 2 - fontHeight / 2;
				TextOut(memDC, game.field[i][j].pos.x + delX, game.field[i][j].pos.y + delY, buff, wcslen(buff));

				SelectObject(memDC, oldFont);
				DeleteObject(oldFont);
				DeleteObject(newFont);
				DeleteObject(hBlockBrush);

			}
		}
	}
}


void Painter::DrawMainElements(int currScore, int bestScore) 
{
	SelectObject(memDC, hBackgroundBrush);
	Rectangle(memDC, -1, -1, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Задаем цвет текста и фона текста для 
	SetBkColor(memDC, RGB(143, 122, 101));
	SetTextColor(memDC, RGB(227, 216, 205));
	wchar_t buffer[100];
	wchar_t scoreTxtBuffer[100];

	HBRUSH hBrush;
	
	HFONT oldFont, newFont;
	newFont = CreateFont(20, 0, 0, 0, FW_DEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
	oldFont = static_cast<HFONT>(SelectObject(memDC, newFont));

#pragma region Прямоугольник с текущим счетом 
	swprintf_s(buffer, L"SCORE");
	swprintf_s(scoreTxtBuffer, L"%d", currScore);
	int txtScoreLenght = wcslen(scoreTxtBuffer);
	hBrush = CreateSolidBrush(RGB(143, 122, 101));
	SelectObject(memDC, hBrush);

	RoundRect(memDC, 20, 10, txtScoreLenght + 120, 70, 15, 13);
	TextOut(memDC, 20 + GetDeltaX(txtScoreLenght + 100, memDC, buffer), 20, buffer, wcslen(buffer));//title

	newFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
	HFONT tmp = static_cast<HFONT>(SelectObject(memDC, newFont));
	TextOut(memDC, 20 + GetDeltaX(txtScoreLenght + 100, memDC, scoreTxtBuffer), 40, scoreTxtBuffer, wcslen(scoreTxtBuffer));//сам счет
	SelectObject(memDC, tmp);
#pragma endregion

#pragma region Прямоугольник с лучшим счетом
	swprintf_s(buffer, L"BEST");
	swprintf_s(scoreTxtBuffer, L"%d", bestScore);
	txtScoreLenght = wcslen(scoreTxtBuffer);
	hBrush = CreateSolidBrush(RGB(143, 122, 101));
	SelectObject(memDC, hBrush);

	RoundRect(memDC, WINDOW_WIDTH - 140 - txtScoreLenght, 10, WINDOW_WIDTH - 40, 70, 15, 13);

	int delX = GetDeltaX(100 + txtScoreLenght, memDC, buffer);
	TextOut(memDC, WINDOW_WIDTH - 140 - txtScoreLenght + delX, 20, buffer, wcslen(buffer));//title
	newFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
	SelectObject(memDC, newFont);

	delX = GetDeltaX(txtScoreLenght + 100, memDC, scoreTxtBuffer);
	TextOut(memDC, WINDOW_WIDTH - 140 - txtScoreLenght + delX, 40, scoreTxtBuffer, wcslen(scoreTxtBuffer));//сам счет
#pragma endregion

	//Внешний квадрат (темносерый)
	hBrush = CreateSolidBrush(RGB(187, 173, 160));
	SelectObject(memDC, hBrush);
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

	//Внутренние квадраты(светлосерые
	hBrush = CreateSolidBrush(RGB(205, 193, 180));
	SelectObject(memDC, hBrush);
	int tmpXPadding = 0;
	int tmpYPadding = 0;
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			RoundRect
			(
				memDC, 
				PLAYINGFIELD_MARGIN + j * TILE_SIZE + currTilePadding + tmpXPadding,
				WINDOW_MARGIN + i * TILE_SIZE + currTilePadding + tmpYPadding,
				PLAYINGFIELD_MARGIN + j * TILE_SIZE + currTilePadding + TILE_SIZE + tmpXPadding,
				WINDOW_MARGIN + i * TILE_SIZE + currTilePadding + TILE_SIZE + tmpYPadding,
				15, 
				13);
			tmpXPadding += currTilePadding;
		}
		tmpXPadding = 0;
		tmpYPadding += currTilePadding;
	}

	SelectObject(memDC, oldFont);
	DeleteObject(oldFont);
	DeleteObject(newFont);
	DeleteObject(hBrush);
}


Painter::~Painter()
{
	DeleteObject(Painter::hBackgroundBrush);
	DeleteDC(Painter::memDC);
}