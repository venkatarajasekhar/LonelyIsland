#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	Engine *pEngine = Engine::getInstance();
	pEngine->InitWithProgramInstance(hInstance);
	if (pEngine->isInvalid())
		return 0;
	pEngine->Run();
	pEngine->Release();

	return 0;
}