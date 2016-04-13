#include "Engine.h"

HRESULT Engine::CreateUserWindow(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEX Window;

	Window.cbSize = sizeof(WNDCLASSEX);
	Window.style = CS_HREDRAW | CS_VREDRAW;
	Window.lpfnWndProc = (WNDPROC)&pMessageManager->MessageDispatcher;
	Window.cbClsExtra = 0;
	Window.cbWndExtra = 0;
	Window.hInstance = hInstance;
	Window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
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

	HWND hWnd = CreateWindow(lpszWindowClass, lpszTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100, NULL, NULL,
		hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"),
			plszAppName, NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return 0;
}

void Engine::Run() {
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}