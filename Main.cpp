#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	Engine *pEngine = Engine::getInstance();
	pEngine->InitWithProgramInstance(hInstance);
	pEngine->Run();

	return 0;
}