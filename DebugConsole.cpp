#include "DebugConsole.h"
#include "framework.h"

DebugConsole::DebugConsole(bool open): open(open) {
	if (!open) return;

	AllocConsole();
	freopen_s(&pCout, "conout$", "w+t", stdout); 
	freopen_s(&pIn, "conin$", "r+t", stdin);
}

DebugConsole::~DebugConsole() {
	if (!open) return;
	fclose(pIn);
	fclose(pCout);
	FreeConsole();
}