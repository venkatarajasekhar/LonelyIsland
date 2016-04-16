#pragma once

#include <memory>
#include <tchar.h>
#include <windows.h>

#include "ErrorManager.h"
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

	bool Invalid;

	Engine(const Engine&) = delete;
	Engine(const Engine&&) = delete;
	Engine &operator=(const Engine) = delete;

	Engine() : Opts(new Options()), Renderer(new DXRenderer()), Invalid(true) { }

	const TCHAR *lpszWindowClass = _T("lonelyisland");
	const TCHAR *lpszTitle = _T("DirectX App");
	const TCHAR *plszAppName = _T("Lonely Island");

	HRESULT CreateUserWindow();

public:

	bool isInvalid() {
		return Invalid;
	}

	void InitWithProgramInstance(HINSTANCE hInstance) {
		hProgramInstance = hInstance;
		CreateUserWindow();
		Renderer->Init(hInstance, hWindow);
		if (Renderer->isInvalid()) {
			EmitError(_T("Cannot initialize engine!"));
			return;
		}
		Invalid = false;
	}

	static LRESULT CALLBACK MessageDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Run();

	void Release() {
		delete Opts;
		Renderer.release();
		Invalid = true;
	}
};
