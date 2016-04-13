#pragma once

class MessageManager : public Singleton<MessageManager> {
public:
	static LRESULT CALLBACK MessageDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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

};
