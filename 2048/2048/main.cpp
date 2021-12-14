#include <Windows.h>
#include "resource.h"
#include "constants.h"
#include "Painter.h"
#include "Game.h"


RECT clientRect;
HWND hWnd{};
HWND hWnd_btn{};

int windowWidth;
int windowHeight;

int FIELD_SIZE;
float TILE_SIZE;
int currTilePadding;

bool isEnd;

Painter* painter;
Game* game;

bool isRepeat = false;


void UpdateWinSizeParams(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE, LPSTR lpCmdLine, int nCmdShow);


void StartGameInitialize(int size);
void DrawButton(LPDRAWITEMSTRUCT lpInfo);
void ObjRefClean();



void UpdateWinSizeParams(HWND hWnd) {
	GetClientRect(hWnd, &clientRect);
	windowWidth = clientRect.right - clientRect.left;
	windowHeight = clientRect.bottom - clientRect.top;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	int instance;
	int n;
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		DrawButton((LPDRAWITEMSTRUCT)lParam);
		return true;
	}
	case WM_COMMAND:
		if (lParam == (LPARAM)hWnd_btn) {
			game->SetLastHistoryToField();
			InvalidateRect(hWnd, NULL, 1);
			break;
		}
		switch (LOWORD(wParam))
		{
		case ID_KEY_UP:
			game->SaveIntoHistory();
			if (game->KeyUpHandler(false))
				game->RandomizeValueOneTile();
			else
				game->currHistoryPos--;

			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_DOWN:
			game->SaveIntoHistory();
			if (game->KeyDownHandler(false))
				game->RandomizeValueOneTile();
			else
				game->currHistoryPos--;

			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_LEFT:
			game->SaveIntoHistory();
			if (game->KeyLeftHandler(false))
				game->RandomizeValueOneTile();
			else
				game->currHistoryPos--;

			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_RIGHT:
			game->SaveIntoHistory();
			if (game->KeyRightHandler(false))
				game->RandomizeValueOneTile();
			else
				game->currHistoryPos--;

			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_BACK:
			game->SetLastHistoryToField();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_CONSTROLS_NEWGAME:
			SaveBestScore(game->bestScore);
			game->StartNewGame(hWnd);
			break;
		case ID_MODES_3X3:
			game->SaveResultsInFile();
			ObjRefClean();
			StartGameInitialize(3);
			painter->SetHWND(hWnd);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_MODES_4X4:
			game->SaveResultsInFile();
			ObjRefClean();
			StartGameInitialize(4);
			painter->SetHWND(hWnd);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_MODES_5X5:
			game->SaveResultsInFile();
			ObjRefClean();
			StartGameInitialize(5);
			painter->SetHWND(hWnd);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_MODES_6X6:
			game->SaveResultsInFile();
			ObjRefClean();
			StartGameInitialize(6);
			painter->SetHWND(hWnd);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_HELP:
			MessageBox(hWnd, TEXT("©Marian Kozlovskiy, 2021\n"), TEXT("Help"), MB_OK);

		case ID_CONSTROLS_RESETALLRESULTS:
			n = MessageBox(hWnd, TEXT("Do you realy wants to clean all results?"), TEXT("GAME OVER"), MB_YESNO);
			if (n == IDYES) {
				CreateNewBestScoreFile(4);
				game->StartNewGame(hWnd);
				for (int i = 3; i <= 6; i++) {
					CreateNewLastSaveFile(i);
					CreateNewBestScoreFile(i);
				}
			}
		default:
			break;
		}
	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_PAINT:
		if (game->bestScore < game->score) { game->bestScore = game->score; };

			
		painter->Redraw(*game);

		instance = game->IsGameOver();
		switch (instance)
		{
		case 0: //Проигрыш, даем выбор игроку, либо начать новую, либо откатить ход
			n = MessageBox(hWnd, TEXT("YOU LOSE!\nDo you want to start NEW GAME?"), TEXT("GAME OVER"), MB_YESNO);
			if (n == IDYES) {
				SaveBestScore(game->bestScore);
				game->StartNewGame(hWnd);
			}
			else {
				game->SetLastHistoryToField();
				InvalidateRect(hWnd, NULL, 1);
			}
			break;
		case 1: //Игрок собрал 2048 первый раз, оповещаем об этом
			n = MessageBox(hWnd, TEXT("YOU WIN!\nSet INFINITE mode"), TEXT("YOU WIN"), MB_OK);
			break;
		default:
			break;
		}
	break;
	case WM_DESTROY:
		game->SaveResultsInFile();
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstanse;

	wc.lpfnWndProc = WndProc;

	wc.lpszClassName = szClassWindow;
	wc.lpszMenuName = TEXT("IDR_MENU1");
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
		return EXIT_FAILURE;

	//Инициализация выпадающего меню и акселераторов созданного в ресурсах
	HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	HACCEL hAccel = LoadAccelerators(hInstanse, MAKEINTRESOURCE(IDR_ACCELERATOR2));



	hWnd = CreateWindowEx
	(
		0,
		wc.lpszClassName,
		szClassWindow,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		nullptr,
		hMenu,
		wc.hInstance,
		nullptr
	);
	if (hWnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	hWnd_btn = CreateWindow(L"Button", L"Btn_turn_back", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_OWNERDRAW | BS_PUSHBUTTON, PLAYINGFIELD_MARGIN, 120, 100, 35, hWnd, NULL, hInstanse, NULL);
	HRGN rgn = CreateRoundRectRgn(0, 0, 100, 35, 20, 15);
	SetWindowRgn(hWnd_btn, rgn, TRUE);

	UpdateWinSizeParams(hWnd);

	SetMenu(hWnd, hMenu);
	

	StartGameInitialize(4);

	painter->SetHWND(hWnd);
	//game->StartNewGame(hWnd);
	InvalidateRect(hWnd, NULL, 1);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	ObjRefClean();
	return static_cast<int>(msg.wParam);
}

void StartGameInitialize(int size)
{
	FIELD_SIZE = size;
	currTilePadding = TILE_PADDING[FIELD_SIZE - 3];
	TILE_SIZE = PLAYINGFIELD_SIZE / FIELD_SIZE - 2 * currTilePadding;

	painter = new Painter();
	game = new Game(hWnd);
}


void DrawButton(LPDRAWITEMSTRUCT lpInfo) {
	HGDIOBJ obj;
	HBRUSH noactive = CreateSolidBrush(RGB(143, 122, 101)), focus = CreateSolidBrush(RGB(0, 255, 0)), select = CreateSolidBrush(RGB(64, 57, 50));
	HBITMAP bm;
	HDC Owner;

	Owner = CreateCompatibleDC(lpInfo->hDC);
	bm = CreateCompatibleBitmap(lpInfo->hDC, lpInfo->rcItem.right, lpInfo->rcItem.bottom);
	obj = SelectObject(Owner, bm);
	FillRect(Owner, &lpInfo->rcItem, noactive);
	switch (lpInfo->itemAction)
	{
	case ODA_FOCUS:
		if (lpInfo->itemState & ODS_FOCUS)
			FillRect(Owner, &lpInfo->rcItem, focus);
		if (lpInfo->itemState & ODS_CHECKED)
			FillRect(Owner, &lpInfo->rcItem, noactive);
		break;
	case ODA_SELECT:
		if (lpInfo->itemState & ODS_SELECTED)
			FillRect(Owner, &lpInfo->rcItem, select);
		break;
	case ODA_DRAWENTIRE:
		FillRect(Owner, &lpInfo->rcItem, noactive);
		break;
	}

	wchar_t wtext[] = L"Turn back";
	SetBkMode(Owner, TRANSPARENT);
	SetTextColor(Owner, RGB(227, 216, 205));

	HFONT oldFont, newFont;
	newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
	oldFont = (HFONT)SelectObject(lpInfo->hDC, newFont);
	TextOut(Owner, GetDeltaX(100, lpInfo->hDC, wtext) - 5, 9, wtext, wcslen(wtext));
	SelectObject(lpInfo->hDC, oldFont);

	BitBlt(lpInfo->hDC, 0, 0, lpInfo->rcItem.right, lpInfo->rcItem.bottom, Owner, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(Owner, obj));
	DeleteDC(Owner);
}

void ObjRefClean()
{
	delete(painter);
	delete(game);
}
