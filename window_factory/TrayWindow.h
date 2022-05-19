#pragma once

#include "BaseWindow.h"
#include "../imgui/imgui.h"

#include <d3d12.h>
#include <shellapi.h>

#define WM_TRAY (WM_USER + 0x100)

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


	ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
};

