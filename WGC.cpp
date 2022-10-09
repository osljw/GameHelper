#include "WGC.h"
#include "imgui/imgui.h"
#include <DirectXTex.h>


WGC::WGC()
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
}

WGC::~WGC()
{
    winrt::uninit_apartment();
}

bool WGC::init(HWND hwnd) {
    if (hwnd == nullptr) {
        std::cout << "HWND not find:" << std::hex << hwnd << std::endl;
        return false;
    }
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
        return false;
    }

    hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    if (FAILED(hr)) {
        std::cout << "IDXGIDevice failed" << std::endl;
        return false;
    }



    winrt::com_ptr<IInspectable> inspectable;
    hr = CreateDirect3D11DeviceFromDXGIDevice(dxgiDevice,
        inspectable.put());
    if (FAILED(hr)) {
        std::cout << "CreateDirect3D11DeviceFromDXGIDevice failed" << std::endl;
        return false;
    }
    device = inspectable.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>();

    //// DispatcherQueue
    //winrt::Windows::System::DispatcherQueue dispatcher_queue = nullptr;
    //dispatcher_queue = winrt::Windows::System::DispatcherQueue::GetForCurrentThread();
    //WINRT_VERIFY(dispatcher_queue != nullptr);


    DispatcherQueueOptions options{
        sizeof(DispatcherQueueOptions),
        DQTYPE_THREAD_CURRENT,
        //DQTYPE_THREAD_DEDICATED,
        DQTAT_COM_STA
    };

    winrt::check_hresult(
        CreateDispatcherQueueController(
            options,
            reinterpret_cast<
            ABI::Windows::System::IDispatcherQueueController**>(winrt::put_abi(controller))
        )
    );
    //queue = controller.DispatcherQueue();
    //std::cout << "DispatcherQueue create" << std::endl;

    //// Enqueue our capture work on the dispatcher
    //auto success = queue.TryEnqueue([=]() -> void
    //    {
    //        //g_app->Initialize(root);
    //    });
    //WINRT_VERIFY(success);

    // ======================== create GraphicsCaptureItem ==================================
    auto activation_factory = winrt::get_activation_factory<
        winrt::Windows::Graphics::Capture::GraphicsCaptureItem>();
    auto interop_factory =
        activation_factory.as<IGraphicsCaptureItemInterop>();

    //auto interop_factory = winrt::get_activation_factory<
    //    winrt::Windows::Graphics::Capture::GraphicsCaptureItem, IGraphicsCaptureItemInterop>();


    winrt::Windows::Graphics::Capture::GraphicsCaptureItem item = { nullptr };
    hr = interop_factory->CreateForWindow(
        hwnd,
        winrt::guid_of<ABI::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
        reinterpret_cast<void**>(winrt::put_abi(item))
    );
    if (FAILED(hr)) {
        std::cout << "CreateForWindow failed" << std::endl;
        return false;
    }
    // ===========================================================================================
    const winrt::Windows::Graphics::SizeInt32 size = item.Size();

    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = size.Width;
    texDesc.Height = size.Height;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.SampleDesc = { 1, 0 };
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // must have
    texDesc.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D(&texDesc, NULL, &d3d11Texture);
    if (FAILED(hr)) {
        std::cout << "CreateTexture2D failed" << std::endl;
        return false;
    }


    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    hr = g_pd3dDevice->CreateShaderResourceView(d3d11Texture, &srvDesc, &d3d11SRV);

    if (FAILED(hr)) {
        std::cout << "CreateShaderResourceView failed: " << std::hex << hr << std::endl;
    }

    // create Direct3D11CaptureFramePool
    frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(
        device,
        winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
        2,
        size);
    //frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::CreateFreeThreaded(
    //    device,
    //    static_cast<winrt::Windows::Graphics::DirectX::DirectXPixelFormat>(DXGI_FORMAT_B8G8R8A8_UNORM),
    //    2,
    //    size);

    session = frame_pool.CreateCaptureSession(item);

    frame_arrived = frame_pool.FrameArrived(
        winrt::auto_revoke,
        { this, &WGC::on_frame_arrived });

    //frame_pool.FrameArrived({ this, &WGC::on_frame_arrived });
    session.StartCapture();
    std::cout << "start capture ========= " << std::endl;

    return true;
}

struct __declspec(uuid("A9B3D012-3DF2-4EE3-B8D1-8695F457D3C1"))
    IDirect3DDxgiInterfaceAccess : ::IUnknown {
    virtual HRESULT __stdcall GetInterface(GUID const& id,
        void** object) = 0;
};

template<typename T>
static winrt::com_ptr<T> GetDXGIInterfaceFromObject(
    winrt::Windows::Foundation::IInspectable const& object)
{
    auto access = object.as<IDirect3DDxgiInterfaceAccess>();
    winrt::com_ptr<T> result;
    winrt::check_hresult(
        access->GetInterface(winrt::guid_of<T>(), result.put_void()));
    return result;
}

void WGC::on_frame_arrived(winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender,
    winrt::Windows::Foundation::IInspectable const&)
{
    //std::cout << "on_frame_arrived: " << std::this_thread::get_id() << std::endl;

    const winrt::Windows::Graphics::Capture::Direct3D11CaptureFrame frame = sender.TryGetNextFrame();
    const winrt::Windows::Graphics::SizeInt32 frame_content_size = frame.ContentSize();

    bool size_changed = false;
    if (frame_content_size.Width != m_lastSize.Width ||
        frame_content_size.Height != m_lastSize.Height) {
        size_changed = true;
        m_lastSize = frame_content_size;
    }
    
    winrt::com_ptr<ID3D11Texture2D> frame_surface = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());

    /* need GetDesc because ContentSize is not reliable */
    D3D11_TEXTURE2D_DESC desc;
    frame_surface->GetDesc(&desc);
    captureHeight = frame_content_size.Height;
    captureWidth = frame_content_size.Width;

    //std::cout << "on_frame_arrived =========" << captureHeight << ", " << captureWidth << ", size:" << frame_content_size.Height << ", " << frame_content_size.Width << std::endl;

    HRESULT hr;

    g_pd3dDeviceContext->CopyResource((ID3D11Resource*)d3d11Texture, (ID3D11Resource*)frame_surface.get());

    //DirectX::ScratchImage image;
    //hr = DirectX::CaptureTexture(g_pd3dDevice, g_pd3dDeviceContext, (ID3D11Resource*)d3d11Texture, image);
    //if (FAILED(hr)) {
    //    std::cout << "DirectX::CaptureTexture failed" << std::endl;
    //}
    //DirectX::SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, L"test.dds");


    //if (d3d11SRV) d3d11SRV->Release();
    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    hr = g_pd3dDevice->CreateShaderResourceView(d3d11Texture, &srvDesc, &d3d11SRV);
    
    if (FAILED(hr)) {
        std::cout << "CreateShaderResourceView failed: " << std::hex << hr << std::endl;
    }

    if (size_changed) {
        //std::cout << "========size_changed: recreate" << std::endl;
        frame_pool.Recreate(
            device,
            static_cast<winrt::Windows::Graphics::DirectX::DirectXPixelFormat>(desc.Format),
            2,
            m_lastSize);
    }
}

bool WGC::render()
{
    if (show_window) {

        //session.StartCapture();

        ImGui::Begin("desktop captur", &show_window);

        //std::cout << "captureWidth, captureHeight" << captureWidth << "," << captureHeight << ", format:" << std::endl;
        if (d3d11SRV) {
            ImGui::Image((ImTextureID)d3d11SRV, ImVec2(captureWidth, captureHeight));
        }

        //ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)d3d11SRV, ImVec2(0, 0), ImVec2(captureWidth, captureHeight));

        ImGui::End();
    }

    return true;
}

//winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Graphics::Capture::GraphicsCaptureItem> WGC::StartCaptureWithPickerAsync()
//{
//    auto item = co_await m_capturePicker.PickSingleItemAsync();
//    if (item)
//    {
//        // We might resume on a different thread, so let's resume execution on the
//        // main thread. This is important because SimpleCapture uses 
//        // Direct3D11CaptureFramePool::Create, which requires the existence of
//        // a DispatcherQueue. See CaptureSnapshot for an example that uses 
//        // Direct3D11CaptureFramePool::CreateFreeThreaded, which doesn't now have this
//        // requirement. See the README if you're unsure of which version of 'Create' to use.
//        //co_await m_mainThread;
//        StartCaptureFromItem(item);
//    }
//
//    co_return item;
//}


//winrt::fire_and_forget ShutdownAndThenPostQuitMessage(winrt::Windows::System::DispatcherQueueController const& controller, int exitCode)
//{
//    auto queue = controller.DispatcherQueue();
//    co_await controller.ShutdownQueueAsync();
//    co_await queue;
//    PostQuitMessage(exitCode);
//    co_return;
//}