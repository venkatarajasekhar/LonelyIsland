#pragma once

#include <windows.h>
#include <tchar.h>

#include "Singleton.h"
#include "MessageManager.h"

class Engine : public Singleton<Engine> {
	friend class Singleton<Engine>;

	MessageManager *pMessageManager;

	Engine(const Engine&) = delete;
	Engine(const Engine&&) = delete;

	Engine() : pMessageManager(MessageManager::getInstance()) { }

	const TCHAR *lpszWindowClass = _T("lonelyisland");
	const TCHAR *lpszTitle = _T("DirectX App");
	const TCHAR *plszAppName = _T("Lonely Island");

	HRESULT CreateUserWindow(HINSTANCE hInstance, int nCmdShow);

public:
	void InitWithProgramInstance(HINSTANCE hInstance) {
		CreateUserWindow(hInstance, SW_SHOWMAXIMIZED);
	}

	void Run();
};
