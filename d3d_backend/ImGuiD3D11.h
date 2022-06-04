#pragma once

#include "../framework.h"
#include "D3DBase.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <dxgi1_4.h>




class ImGuiD3D11 : public D3DBase
{
public:
	ImGuiD3D11(HWND hWnd): D3DBase(hWnd) {}
	~ImGuiD3D11() {}

	bool Init();
	void Destroy();

	bool CreateDeviceD3D();
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

	void Begin();
	void End();

	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
	static ID3D11RenderTargetView* g_mainRenderTargetView;
};

