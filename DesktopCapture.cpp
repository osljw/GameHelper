#include "DesktopCapture.h"
#include "d3d_backend/ImGuiD3D11.h"

#include <string.h>
#include <stdint.h>
#include <intrin.h>

//#include <d3d11.h>

#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)
#define AssertHR(hr) Assert(SUCCEEDED(hr))

void DesktopCapture::d3d11_init() {
    HRESULT hr;

    // D3D11CreateDevice or ImGuiD3D11::g_pd3dDevice both can work
    const D3D_FEATURE_LEVEL featureLevelArray[] = { 
        D3D_FEATURE_LEVEL_11_0, 
        D3D_FEATURE_LEVEL_10_0, 
    };
    D3D_FEATURE_LEVEL FeatureLevel;
    hr = D3D11CreateDevice(
        NULL, 
        D3D_DRIVER_TYPE_HARDWARE, NULL, 0, 
        featureLevelArray, ARRAYSIZE(featureLevelArray), 
        D3D11_SDK_VERSION, 
        &g_pd3dDevice, &FeatureLevel, &g_pd3dDeviceContext);
    if (FAILED(hr)) {
        std::cout << "D3D11CreateDevice failed" << std::endl;
        return;
    }

    //hr = ImGuiD3D11::g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    AssertHR(hr);

    IDXGIAdapter* adapter;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
    AssertHR(hr);

    IDXGIOutput* output;
    hr = adapter->EnumOutputs(0, &output);
    AssertHR(hr);

    IDXGIOutput1* output1 = NULL;
    hr = output->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&output1));
    AssertHR(hr);

    //hr = output1->DuplicateOutput((IUnknown*)ImGuiD3D11::g_pd3dDevice, &dxgiDuplication);
    hr = output1->DuplicateOutput((IUnknown*)g_pd3dDevice, &dxgiDuplication);
    AssertHR(hr);

    DXGI_OUTDUPL_DESC desc;
    dxgiDuplication->GetDesc(&desc);

    captureWidth = desc.ModeDesc.Width;
    captureHeight = desc.ModeDesc.Height;
    std::cout << "captureWidth: " << captureWidth << ", captureHeight: " << captureHeight << ", format:" << desc.ModeDesc.Format << std::endl;

    //D3D11_TEXTURE2D_DESC texDesc;
    //ZeroMemory(&texDesc, sizeof(texDesc));
    //texDesc.Width = captureWidth,
    //texDesc.Height = captureHeight;
    //texDesc.Format = desc.ModeDesc.Format;
    //texDesc.MipLevels = 1;
    //texDesc.ArraySize = 1;
    //texDesc.SampleDesc = { 1, 0 };
    //texDesc.Usage = D3D11_USAGE_DEFAULT;
    //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // must have
    //texDesc.CPUAccessFlags = 0;
    //hr = g_pd3dDevice->CreateTexture2D(&texDesc, NULL, &d3d11Texture);
    //if (FAILED(hr)) {
    //    std::cout << "CreateTexture2D failed" << std::endl;
    //    return;
    //}

    output1->Release();
    output->Release();
    adapter->Release();
    dxgiDevice->Release();
    g_pd3dDevice->Release();
}



DesktopCapture::DesktopCapture()
{
    d3d11_init();
}

bool DesktopCapture::get_frame()
{
    DXGI_OUTDUPL_FRAME_INFO info;
    IDXGIResource* resource;
    HRESULT hr = dxgiDuplication->AcquireNextFrame(0, &info, &resource);
    if (FAILED(hr)) {
        std::cout << "AcquireNextFrame failed: " << std::hex << hr << std::endl;
        return false;
    }

    // ¿½±´resourceTexture -> d3d11Texture
    ID3D11Texture2D* resourceTexture;
    resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&resourceTexture));
    //ImGuiD3D11::g_pd3dDeviceContext->CopyResource((ID3D11Resource*)d3d11Texture, (ID3D11Resource*)resourceTexture);
    g_pd3dDeviceContext->CopyResource((ID3D11Resource*)d3d11Texture, (ID3D11Resource*)resourceTexture);
    resourceTexture->Release();

    // Create texture view
    if (d3d11SRV) d3d11SRV->Release();
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    //hr = ImGuiD3D11::g_pd3dDevice->CreateShaderResourceView(d3d11Texture, &srvDesc, &d3d11SRV);
    hr = g_pd3dDevice->CreateShaderResourceView(d3d11Texture, &srvDesc, &d3d11SRV);
    d3d11Texture->Release();
    AssertHR(hr);
    
	return true;
}

bool show_window = true;
bool DesktopCapture::render()
{
    if (show_window) {
        bool ret = get_frame();

        ImGui::Begin("desktop capture", &show_window);

        ImGui::Image((ImTextureID)d3d11SRV, ImVec2(captureWidth, captureHeight));

        //ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)d3d11SRV, ImVec2(0, 0), ImVec2(captureWidth, captureHeight));

        ImGui::End();

        
        dxgiDuplication->ReleaseFrame();
    }


    return true;
}
