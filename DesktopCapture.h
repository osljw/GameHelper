#pragma once

#include "framework.h"

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN


#include "d3d_backend/ImGuiD3D11.h"

#include <dxgi1_2.h>

class DesktopCapture
{
public:
	
	DesktopCapture();

	void d3d11_init();
	bool get_frame();
	bool render();

	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;

	ID3D11Texture2D* d3d11Texture;
	ID3D11ShaderResourceView* d3d11SRV;

	IDXGIDevice* dxgiDevice;
	IDXGIOutputDuplication* dxgiDuplication;
	
	uint32_t captureWidth;
	uint32_t captureHeight;
};

