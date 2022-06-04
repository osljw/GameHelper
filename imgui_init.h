#pragma once


#include "d3d_backend/D3DBase.h"
#include "imgui/imgui.h"



#include <memory>

class ImguiRender {
public:
	static void Begin();
	static void End();
};

class ImguiInit {
public:
	ImguiInit(std::shared_ptr<D3DBase> p_d3d) : p_d3d(p_d3d) {
		imgui_init();
	}

	bool imgui_init();
	void imgui_render();
	void imgui_destroy();

	std::shared_ptr<D3DBase> p_d3d;
};

