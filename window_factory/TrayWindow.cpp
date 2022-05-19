#include "TrayWindow.h"
#include "WindowManager.h"
#include "CrossHairWindow.h"
#include "../imgui_init.h"

#include <strsafe.h>



TrayWindow::TrayWindow(HINSTANCE hInst): BaseWindow(hInst) {
    
}

TrayWindow::~TrayWindow() {
}

bool TrayWindow::create()
{
    // WS_EX_TOOLWINDOW: hWnd窗口不在任务栏显示， 不显示在alt+tab列表中
    // WS_EX_LAYERED | WS_EX_TRANSPARENT: hWnd窗口透明和点击穿透
    hWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        get_winclass().c_str(),
        NULL,
        WS_POPUP,
        0, 0, 0, 0,
        //200, 200,600, 400,
        //0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        //200, 200, height, width,
        //GetSystemMetrics(SM_CXSCREEN)/2 - width/2, GetSystemMetrics(SM_CYSCREEN)/2 - height/2, width, height,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd)
    {
        std::cout << "CreateWindowEx Failed:" << GetLastError() << std::endl;
        return FALSE;
    }

    //hWnd = CreateWindowEx(
    //    //WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
    //    WS_EX_TOPMOST,
    //    get_winclass().c_str(),
    //    NULL,
    //    WS_POPUP,
    //    //0, 0, 0, 0,
    //    200, 200, 600, 400,
    //    nullptr, nullptr, hInstance, nullptr
    //);

    //if (!hWnd)
    //{
    //    std::cout << "CreateWindowEx Failed:" << GetLastError() << std::endl;
    //    return FALSE;
    //}

    //BaseWindow::create();

    return true;
}

bool TrayWindow::show() {
	nid.uVersion = NOTIFYICON_VERSION_4;

	nid.cbSize = sizeof(nid);
	nid.hWnd = get_hwnd();
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// This text will be shown as the icon's tooltip.
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Test application");

	// Load the icon for high DPI.
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY));
	nid.uCallbackMessage = WM_TRAY;

	// Add the icon
	Shell_NotifyIcon(NIM_ADD, &nid);
	// Set the version
	Shell_NotifyIcon(NIM_SETVERSION, &nid);

	BaseWindow::show();

	return true;
}

extern bool show_demo_window;

bool TrayWindow::render() {
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    static float x = 0.5f;
    static float y = 0.5f;

    if (show_main_window) {
        static float f = 0.0f;

        static int counter = 0;

        //ImGui::SetNextWindowBgAlpha(0.0f);

        //std::cout << "window open:" << show_main_window << std::endl;
        ImGui::Begin("Hello, world!", &show_main_window);                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        bool pos_changed = false;
        pos_changed |= ImGui::SliderFloat("x", &x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        pos_changed |= ImGui::SliderFloat("y", &y, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button(u8"居中")) {
            x = 0.5f;
            y = 0.5f;
            pos_changed = true;
        }
        if (pos_changed) {
            CrossHairWindow* crosshair_win = dynamic_cast<CrossHairWindow*>(WindowManager::GetWindow("crosshair"));
            crosshair_win->set_position(x, y);
        }

        ImGui::Image((ImTextureID)my_texture_srv_gpu_handle.ptr, ImVec2((float)image_width, (float)image_height));


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }



    if (show_another_window) {

        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show another simple window.
    //if (show_tray_menu)
    //{
    //    //std::cout << "show_tray_menu" << true << std::endl;

    //    ImGui::OpenPopup("my_select_popup", ImGuiPopupFlags_MouseButtonRight);

    //    if (ImGui::BeginPopup("my_select_popup", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus)) // <-- use last item id as popup id
    //    {

    //        if (ImGui::Button(u8"设置")) {
    //            show_main_window = !show_main_window;
    //            //show_tray_menu = false;
    //        }

    //        HWND crosshair_hwnd = WindowManager::GetWindow("crosshair")->get_hwnd();
    //        if (ImGui::Button(IsWindowVisible(crosshair_hwnd) ? u8"隐藏" : u8"显示")) {
    //            if (IsWindowVisible(crosshair_hwnd)) {
    //                ShowWindow(crosshair_hwnd, SW_HIDE);
    //            }
    //            else {
    //                ShowWindow(crosshair_hwnd, SW_SHOW);
    //            }

    //            //show_tray_menu = false;
    //        }

    //        if (ImGui::Button(u8"退出")) {
    //            //show_tray_menu = false;
    //            PostMessage(hWnd, WM_CLOSE, NULL, NULL);
    //        }

    //        ImGui::EndPopup();
    //    }

    //}

    return true;
}


void TrayWindow::load() {
    load_texture("crosshair.png", my_texture_srv_gpu_handle, image_width, image_height);
}