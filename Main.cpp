#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	Engine *pEngine = Engine::getInstance();
	pEngine->InitWithProgramInstance(hInstance);
	// Main loop
	pEngine->Run();

	return 0;
}