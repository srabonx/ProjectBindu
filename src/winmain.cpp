
#define WIN32_LEAN_AND_MEAN
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include "Include/Bindu.h"

bool CreateMainWindow(HWND& hWnd,HINSTANCE& hInstance,int& nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool e_gameOver;
BINDU::Engine* g_engine;
// Main Entry point of the program
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {

   /*
	*			FOR DETECTING MEMORY LEAKS IN DEBUG MODE
	*/

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg;
	HWND g_hWnd = NULL;

	g_engine = new BINDU::Engine();

	if (!BINDU::game_preload())
	{
		MessageBox(g_hWnd, "Error in game preload!", "Error", MB_OK);
		return 0;
	}

	// Create main window
	if (!CreateMainWindow(g_hWnd, hInstance, nCmdShow))
	{
		MessageBox(g_hWnd, "Error Creating Window!", "Error", MB_OK);
		return 0;
	}

	// Initialize The Engine
	g_engine->setWindowHandle(g_hWnd);

	if (!g_engine->Init())
	{
		MessageBox(g_hWnd, "Error initializing the engine!", "Error", MB_OK);
		return 0;
	}
	BINDU::game_init();

	// Main Message/Game Loop
	e_gameOver = false;
	while (!e_gameOver)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				e_gameOver = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			g_engine->Run();
		}
	}
	BINDU::game_end();
	delete g_engine;
	_CrtDumpMemoryLeaks();
	return 0;
}

bool CreateMainWindow(HWND& hWnd,HINSTANCE& hInstance,int& nCmdShow)
{
	RECT windowRect;
	const char* className = "main_window";
	const char* title = g_engine->getTitle();
	int width = g_engine->getWindowWidth();
	int height = g_engine->getWindowHeight();

	// Set Window Dimension
	windowRect.left = (LONG)0;
	windowRect.right = (LONG)width;
	windowRect.top = (LONG)0;
	windowRect.bottom = (LONG)height;

	// Load Icon
	const char* iconName = g_engine->getAppIcon();
	HICON hIcon;
	if (sizeof(iconName) > 0)
		hIcon = (HICON)LoadImage(GetModuleHandle(0), iconName, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	else
		hIcon = LoadIcon(NULL, IDI_APPLICATION);

	// Create Window class structure
	
	WNDCLASSEX wcx = { sizeof(WNDCLASSEX) };
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = hIcon;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = className;
	wcx.hIconSm = NULL;

	// Register window class
	RegisterClassEx(&wcx);

	// Get monitor display size
	RECT ms;
	GetWindowRect(GetDesktopWindow(), &ms);
	LONG x = ((ms.right - ms.left) / 2) - (width / 2);
	LONG y = ((ms.bottom - ms.top) / 2) - (height / 2);


	hWnd = CreateWindowEx(0,
		className,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		0,0,
		0,0,
		hInstance,
		0
		);

	if (!hWnd)
		return false;
	

	UINT dpi = GetDpiForWindow(hWnd);

	SetWindowPos(hWnd, NULL, x, y,
		static_cast<int>(ceil(width * dpi / 96.0f)),
		static_cast<int>(ceil(height * dpi / 96.0f)),
		SWP_NOMOVE);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return true;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	return (g_engine->MessageLoop(hWnd, msg, wParam, lParam));
}

