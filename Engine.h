#pragma once

#include <tchar.h>
#include <windows.h>

#include "Options.h"
#include "Singleton.h"

class Engine : public Singleton<Engine> {
	friend class Singleton<Engine>;

	Options *Opts;

	Engine(const Engine&) = delete;
	Engine(const Engine&&) = delete;
	Engine &operator=(const Engine) = delete;

	Engine() : Opts(new Options()) { }

	const TCHAR *lpszWindowClass = _T("lonelyisland");
	const TCHAR *lpszTitle = _T("DirectX App");
	const TCHAR *plszAppName = _T("Lonely Island");

	HRESULT CreateUserWindow(HINSTANCE hInstance, int nCmdShow);

public:
	void InitWithProgramInstance(HINSTANCE hInstance) {
		CreateUserWindow(hInstance, SW_SHOWMAXIMIZED);
	}

	static LRESULT CALLBACK MessageDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Run();
};
