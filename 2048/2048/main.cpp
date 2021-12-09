#include <Windows.h>
#include "resource.h"
#include "constants.h"
#include "Painter.h"
#include "Game.h"


RECT clientRect;
HWND hWnd{};

int windowWidth;
int windowHeight;

int FIELD_SIZE;
float TILE_SIZE;
int currTilePadding;

bool isEnd;

Painter* painter;
Game* game;


void UpdateWinSizeParams(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE, LPSTR lpCmdLine, int nCmdShow);


void FirstInitialize();
void FinalClean();



void UpdateWinSizeParams(HWND hWnd) {
	GetClientRect(hWnd, &clientRect);
	windowWidth = clientRect.right - clientRect.left;
	windowHeight = clientRect.bottom - clientRect.top;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_KEY_UP:
			if(game->KeyUpHandler(false))
				game->RandomizeValueOneTile();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_DOWN:
			if (game->KeyDownHandler(false))
				game->RandomizeValueOneTile();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_LEFT:
			if (game->KeyLeftHandler(false))
				game->RandomizeValueOneTile();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_RIGHT:
			if (game->KeyRightHandler(false))
				game->RandomizeValueOneTile();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_KEY_BACK:
			break;
		default:
			break;
		}
		
	case WM_PAINT:
		if (game->bestScore < game->score) { game->bestScore = game->score; };

		painter->Redraw(*game);
		//isEnd = (game->isGameOver()) ? true : false;
		if (game->isGameOver()) {
			int n = MessageBox(hWnd, TEXT("\t    YOU LOOSE!!!\nDo you want to go back?"), TEXT("GAME OVER"), MB_YESNO);
			if (n == IDYES) {
				//GetLastFromHistory();
				InvalidateRect(hWnd, NULL, 1);
			}
			else {
				SaveBestScore(game->bestScore);
				game->StartNewGame(hWnd);
			}
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

	UpdateWinSizeParams(hWnd);

	SetMenu(hWnd, hMenu);
	

	FirstInitialize();

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
	
	FinalClean();
	return static_cast<int>(msg.wParam);
}

void FirstInitialize()
{
	FIELD_SIZE = 4;
	currTilePadding = TILE_PADDING[FIELD_SIZE - 3];
	TILE_SIZE = PLAYINGFIELD_SIZE / FIELD_SIZE - 2 * currTilePadding;

	painter = new Painter();
	game = new Game(hWnd);
}

void FinalClean()
{
	delete(painter);
	delete(game);
}