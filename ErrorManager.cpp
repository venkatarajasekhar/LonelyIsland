#include <windows.h>

#include "ErrorManager.h"

void EmitError(LPCWSTR Message) {
	MessageBox(NULL, Message, _T("Error!"), MB_OK);
}