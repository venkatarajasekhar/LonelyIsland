#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

static TCHAR *szWindowClass = _T("win32app");
static TCHAR *szTitle = _T("Win32 Guided Tour Application");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDisplayDeviceContext;
	TCHAR *greeting = _T("Hello, World!");

	switch (message) {
	case WM_PAINT:
		hDisplayDeviceContext = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, World!"
		// in the top left corner.
		TextOut(hDisplayDeviceContext,
			5, 5, greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	WNDCLASSEX Window;

	Window.cbSize = sizeof(WNDCLASSEX);
	Window.style = CS_HREDRAW | CS_VREDRAW;
	// TODO: Substitute message processing function here
	Window.lpfnWndProc = WndProc;
	Window.cbClsExtra = 0;
	Window.cbWndExtra = 0;
	Window.hInstance = hInstance;
	Window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	Window.hCursor = LoadCursor(NULL, IDC_ARROW);
	Window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	Window.lpszMenuName = NULL;
	Window.lpszClassName = szWindowClass;
	Window.hIconSm = LoadIcon(Window.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&Window)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"), NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100, NULL, NULL,
		hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main loop
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)Msg.wParam;
}