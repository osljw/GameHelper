#pragma once

#include "BaseWindow.h"
#include "../imgui/imgui.h"

#include <vector>
#include <string>
#include <d3d12.h>
#include <shellapi.h>

#define WM_TRAY (WM_USER + 0x100)

class ImageManager {
public:
	ImageManager(std::string path, int index, D3D12_GPU_DESCRIPTOR_HANDLE handle, int image_width, int image_height) {
		this->path = path;
		this->index = index;
		this->my_texture_srv_gpu_handle = handle;
		this->image_width = image_width;
		this->image_height = image_height;
	}

	int index;
	std::string path;

	D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle;
	int image_width = 0;
	int image_height = 0;
};

class TrayWindow : public BaseWindow
{
public:
	TrayWindow(HINSTANCE hInst);
	~TrayWindow();

	NOTIFYICONDATA& get_nid() {
		return nid;
	}

	bool create();
	bool show();
	bool render();

	void render_cursor_pos();
	void render_cursor_size();
	void render_image();

	void load();

	//void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	NOTIFYICONDATA nid;

	bool show_main_window = true;
	bool show_another_window = false;
	bool show_tray_menu = false;
	ImVec2 tray_menu_pos;

	D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle;
	int image_width = 0;
	int image_height = 0;

	std::vector<ImageManager> images;


	ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
};

