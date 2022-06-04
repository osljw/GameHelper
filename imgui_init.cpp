#include "imgui_init.h"
#include "GameHelper.h"
#include "imgui/imgui_impl_win32.h"

bool ImguiInit::imgui_init() {
    //load_texture();;

    //if (!p_d3d->CreateDeviceD3D())
    //{
    //    std::cout << "CreateDeviceD3D failed: " << GetLastError() << std::endl;
    //    p_d3d->CleanupDeviceD3D();
    //    return FALSE;
    //}


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiViewportFlags_NoRendererClear;
    //io.ConfigDockingTransparentPayload = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.0f;
    }


    // Setup Platform/Renderer backends
    //ImGui_ImplWin32_Init(hWnd);
    //ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
    //    DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
    //    g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
    //    g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
    //ImGui_ImplWin32_EnableAlphaCompositing(hWnd);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    //IM_ASSERT(font != NULL);

    return true;
}

// Our state
bool show_demo_window = true;

//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);




void ImguiInit::imgui_destroy() {
    //WaitForLastSubmittedFrame();

    // Cleanup
    //ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImguiRender::Begin()
{
    //ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImguiRender::End()
{
    ImGui::Render();
    
}
