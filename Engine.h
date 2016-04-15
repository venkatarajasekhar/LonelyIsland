#pragma once

#include <memory>
#include <tchar.h>
#include <windows.h>

#include "Options.h"
#include "Singleton.h"
#include "Renderer.h"

class Engine : public Singleton<Engine> {
	friend class Singleton<Engine>;

	Options *Opts;
	HINSTANCE hProgramInstance;
	HWND hWindow;

	// To be sure that all the devices, contextes and buffers are successfully released.
	std::unique_ptr<IRenderer> Renderer;

	Engine(const Engine&) = delete;
	Engine(const Engine&&) = delete;
	Engine &operator=(const Engine) = delete;

	Engine() : Opts(new Options()), Renderer(new DXRenderer()) { }

	const TCHAR *lpszWindowClass = _T("lonelyisland");
	const TCHAR *lpszTitle = _T("DirectX App");
	const TCHAR *plszAppName = _T("Lonely Island");

	HRESULT CreateUserWindow();

public:
	void InitWithProgramInstance(HINSTANCE hInstance) {
		hProgramInstance = hInstance;
		CreateUserWindow();
		Renderer->Init(hInstance, hWindow);
	}

	static LRESULT CALLBACK MessageDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Run();
};
