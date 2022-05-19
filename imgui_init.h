#pragma once

#include "d3d_init.h"

bool imgui_init(HWND hWnd);
void imgui_render();
void imgui_destroy();

class ImguiRender {
public:
	static void Begin();
	static void End();
};

void load_texture(std::string filename, D3D12_GPU_DESCRIPTOR_HANDLE& my_texture_srv_gpu_handle, int& image_width, int& image_height);