#include "Engine.h"

HRESULT Engine::CreateUserWindow() {
	WNDCLASSEX Window;

	Window.cbSize = sizeof(WNDCLASSEX);
	Window.style = CS_HREDRAW | CS_VREDRAW;
	Window.lpfnWndProc = (WNDPROC)&this->MessageDispatcher;
	Window.cbClsExtra = 0;
	Window.cbWndExtra = 0;
	Window.hInstance = hProgramInstance;
	Window.hIcon = LoadIcon(hProgramInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	Window.hCursor = LoadCursor(NULL, IDC_ARROW);
	Window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	Window.lpszMenuName = NULL;
	Window.lpszClassName = lpszWindowClass;
	Window.hIconSm = LoadIcon(Window.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&Window)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"),
			plszAppName, NULL);
		return 1;
	}

	hWindow = CreateWindow(lpszWindowClass, lpszTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		Opts->DisplayWidth, Opts->DisplayHeight, NULL, NULL,
		hProgramInstance, NULL);
	if (!hWindow) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"),
			plszAppName, NULL);
		return 1;
	}

	ShowWindow(hWindow, Opts->WindowType);

	return 0;
}

void Engine::Run() {
	MSG Msg;
	Msg.message = WM_NULL;

	PeekMessage(&Msg, NULL, 0U, 0U, PM_NOREMOVE);
	while (WM_QUIT != Msg.message) {
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		} else
			Renderer->Render();
	}
}

LRESULT CALLBACK Engine::MessageDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDisplayDeviceContext;
	const TCHAR *greeting = _T("Hello, World!");

	switch (message) {
	case WM_PAINT:
		hDisplayDeviceContext = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, World!"
		// in the top left corner.
		TextOut(hDisplayDeviceContext, 5, 5, greeting, _tcslen(greeting));
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
