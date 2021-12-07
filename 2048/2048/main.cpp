#include <Windows.h>
#include "resource.h"
#include "constants.h"
#include"Painter.h"


RECT clientRect;

int windowWidth;
int windowHeight;

Painter* painter = new Painter;


void UpdateWinSizeParams(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE, LPSTR lpCmdLine, int nCmdShow);
void FinalClean();



void UpdateWinSizeParams(HWND hWnd) {
	GetClientRect(hWnd, &clientRect);
	windowWidth = clientRect.right - clientRect.left;
	windowHeight = clientRect.bottom - clientRect.top;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	/*case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_KEY_UP:

		case ID_KEY_DOWN:

		case ID_KEY_LEFT:

		case ID_KEY_RIGHT:

		case ID_KEY_BACK:

		default:
			break;
		}
	case WM_PAINT:
*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstanse, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hWnd{};
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
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
	
	//painter->SetHWND(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		//if (!TranslateAccelerator(hWnd, hAccel, &msg)) 
		//{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		//}
	}

	FinalClean();
	return static_cast<int>(msg.wParam);
}

void FinalClean()
{
	delete(painter);
}