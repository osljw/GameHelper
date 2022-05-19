#include "Tray.h"
#include "Resource.h"
#include <strsafe.h>
//#include <commctrl.h>



Tray::Tray(HINSTANCE hInst, HWND hWnd) {
	nid.uVersion = NOTIFYICON_VERSION_4;

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// This text will be shown as the icon's tooltip.
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Test application");

	// Load the icon for high DPI.
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAY));
	nid.uCallbackMessage = WM_TRAY;

	// Add the icon
	Shell_NotifyIcon(NIM_ADD, &nid);
	// Set the version
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

Tray::~Tray() {
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void Tray::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        case WM_TRAY: {
            int wmId = LOWORD(lParam);
            switch (wmId) {
                case WM_RBUTTONDOWN: {
                    std::cout << "tray right click: " << std::endl;
                    POINT pt;
                    GetCursorPos(&pt);
                    HMENU hMenu;
                    hMenu = CreatePopupMenu();
                    AppendMenu(hMenu, MF_STRING, 1234, TEXT("ÏÔÊ¾"));
                    AppendMenu(hMenu, MF_STRING, 2, TEXT("ÍË³ö"));
                    TrackPopupMenu(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, NULL, hWnd, NULL);
                    break;
                }
                default:
                    DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    }
}