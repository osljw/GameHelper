#include "App.h"

#include "window_factory/CrossHairWindow.h"
#include "Window_factory/FoolWindow.h"

App::App(HINSTANCE hInstance)
	: main_window(hInstance)
{
	main_window.run();

	// init imgui on main window hwnd
	imgui_manager.Init(main_window.get_hwnd());



	//windows.push_back(std::make_shared<CrossHairWindow>(hInstance));
	windows.push_back(std::make_shared<FoolWindow>(hInstance));

	for (auto window : windows) {
		window->run();
	}
}

void App::Render()
{
	imgui_manager.Begin();

	for (auto window : windows) {
		window->render();
	}

	imgui_manager.End();
}
