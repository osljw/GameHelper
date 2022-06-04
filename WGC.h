#pragma once

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

// WinRT
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3d11.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Security.Authorization.AppCapabilityAccess.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.UI.Popups.h>

#include <DispatcherQueue.h>
#include <windows.graphics.capture.interop.h>
#include <windows.graphics.directx.direct3d11.interop.h>

#include <iostream>
#include <mutex>

class WGC
{
public:
	WGC();
	~WGC();

	bool init(HWND window);

	void on_frame_arrived(winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender, winrt::Windows::Foundation::IInspectable const&);

	bool render();

	//winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Graphics::Capture::GraphicsCaptureItem> StartCaptureWithPickerAsync();

	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;
	IDXGIDevice* dxgiDevice;

	winrt::Windows::System::DispatcherQueueController controller = nullptr;
	winrt::Windows::System::DispatcherQueue queue = nullptr;

	winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice device;
	winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool frame_pool = nullptr;
	winrt::Windows::Graphics::Capture::GraphicsCaptureSession session = nullptr;
	winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::FrameArrived_revoker frame_arrived;
	winrt::Windows::Graphics::SizeInt32 m_lastSize;

	//winrt::com_ptr<IDXGISwapChain1> m_swapChain{ nullptr };
	ID3D11Texture2D* d3d11Texture;
	ID3D11ShaderResourceView* d3d11SRV;

	bool show_window = true;
	uint32_t captureWidth;
	uint32_t captureHeight;

	//ID3D11ShaderResourceView* d3d11SRV[2];
	//int idx = 0;
	//std::mutex mux;
};

