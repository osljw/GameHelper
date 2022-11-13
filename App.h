#pragma once

#include <vector>
#include <memory>

#include "d3d_backend/ImGuiD3D11.h"
#include "window_factory/BaseWindow.h"
#include "window_factory/ImGuiBaseWindow.h"

class App
{
public:
	App(HINSTANCE hInstance);

	void Render();

public:
	BaseWindow main_window;		// App need one main window (can be invisible)
	ImGuiD3D11 imgui_manager;
	std::vector<std::shared_ptr<ImGuiBaseWindow>> windows;
};

