#pragma once
#include "framework.h"
#include "shellapi.h"

#define WM_TRAY (WM_USER + 0x100)

class Tray
{
public:
	Tray(HINSTANCE hInst, HWND hWnd);
	~Tray();



	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	NOTIFYICONDATA nid;
};

