#pragma once
#include "../framework.h"

class D3DBase
{
public:
	D3DBase() {}
	virtual ~D3DBase() {}

	virtual bool CreateDeviceD3D() = 0;
	virtual void CleanupDeviceD3D() = 0;
	virtual void CreateRenderTarget() = 0;
	virtual void CleanupRenderTarget() = 0;

	HWND GetHWND() {
		return hWnd;
	}

	HWND hWnd;
};

