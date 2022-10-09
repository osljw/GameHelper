#pragma once

#include "BaseWindow.h"
#include <thread>

class FoolWindow : public BaseWindow
{
public:
	FoolWindow(HINSTANCE hInstance) : BaseWindow(hInstance) {};
	~FoolWindow() {}

	bool render();

	bool show_window = true;
	std::thread task;
};

