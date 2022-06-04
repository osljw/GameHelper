#pragma once

#include "framework.h"
#include "window_factory/BaseWindow.h"

#include <dwmapi.h>

class WindowCapture : public BaseWindow
{
public:
	WindowCapture(HINSTANCE hInstance) :BaseWindow(hInstance) {}
	bool create();
	bool show();
	bool take_photo(std::wstring path);
};

