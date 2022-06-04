#include "TrayWindow.h"
#include "WindowManager.h"
#include "CrossHairWindow.h"
#include "../imgui_init.h"

#include <strsafe.h>
#include <commdlg.h>
#include <filesystem>


//std::wstring to_wstring(const std::string& str)
//{
//    using convert_typeX = std::codecvt_utf8<wchar_t>;
//    std::wstring_convert<convert_typeX, wchar_t> converterX;
//
//    return converterX.from_bytes(str);
//}
//
//std::string to_string(const std::wstring& wstr)
//{
//    using convert_typeX = std::codecvt_utf8<wchar_t>;
//    std::wstring_convert<convert_typeX, wchar_t> converterX;
//
//    return converterX.to_bytes(wstr);
//}

TrayWindow::TrayWindow(HINSTANCE hInst): BaseWindow(hInst) {
    
}

TrayWindow::~TrayWindow() {
}

bool TrayWindow::create()
{
    // WS_EX_TOOLWINDOW: hWnd窗口不在任务栏显示， 不显示在alt+tab列表中
    // WS_EX_LAYERED | WS_EX_TRANSPARENT: hWnd窗口透明和点击穿透
    hWnd = CreateWindowEx(
        //WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        get_winclass().c_str(),
        NULL,
        WS_POPUP,
        0, 0, 0, 0,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd)
    {
        std::cout << "CreateWindowEx Failed:" << GetLastError() << std::endl;
        return FALSE;
    }

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



    if (show_main_window) {
        static float f = 0.0f;

        static int counter = 0;

        //ImGui::SetNextWindowBgAlpha(0.0f);

        //std::cout << "window open:" << show_main_window << std::endl;
        ImGui::Begin("Hello, world!", &show_main_window);                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);



        render_cursor_pos();
        render_cursor_size();
        render_image();


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
    //std::string path = "resource\\crosshair.png";
    //load_texture(path, 1, my_texture_srv_gpu_handle, image_width, image_height);
    //images.emplace_back(path, 1, my_texture_srv_gpu_handle, image_width, image_height);

    //for (int i = 0; i < 30; i++) {
    //    load_texture(path, my_texture_srv_gpu_handle, image_width, image_height);
    //    images.emplace_back(path, my_texture_srv_gpu_handle, image_width, image_height);
    //}
}

void TrayWindow::render_cursor_pos() {
    static float x = 0.5f;
    static float y = 0.5f;

    ImGui::NewLine();
    ImGui::Text((char*)u8"光标位置");
    ImGui::Separator();

    bool pos_changed = false;
    pos_changed |= ImGui::SliderFloat("x", &x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    if (ImGui::Button((char*)u8"水平居中")) {
        x = 0.5f;
        pos_changed = true;
    }

    ImGui::NewLine();
    pos_changed |= ImGui::SliderFloat("y", &y, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    if (ImGui::Button((char*)u8"垂直居中")) {
        y = 0.5f;
        pos_changed = true;
    }

    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


    if (pos_changed) {
        CrossHairWindow* crosshair_win = dynamic_cast<CrossHairWindow*>(WindowManager::GetWindow("crosshair"));
        crosshair_win->set_position(x, y);
    }
}

void TrayWindow::render_cursor_size() {
    ImGui::NewLine();
    ImGui::Text((char*)u8"光标大小");
    ImGui::Separator();

    static int x = 15;
    static int y = 15;
    static float xy = 1.0f;


    bool size_changed = false;
    size_changed |= ImGui::SliderInt("width", &x, 0, GetSystemMetrics(SM_CXSCREEN));
    ImGui::SameLine();
    if (ImGui::Button((char*)u8"宽度")) {
        x = 15;
        size_changed = true;
    }

    ImGui::NewLine();
    size_changed |= ImGui::SliderInt("height", &y, 0, GetSystemMetrics(SM_CYSCREEN));            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    if (ImGui::Button((char*)u8"高度")) {
        y = 15;
        size_changed = true;
    }

    //ImGui::NewLine();
    //if (ImGui::SliderFloat("scale", &xy, 0.0f, 1.0f)) {
    //    x = GetSystemMetrics(SM_CXSCREEN) * xy;
    //    y = GetSystemMetrics(SM_CYSCREEN) * xy;
    //    size_changed = true;
    //}
    //ImGui::SameLine();
    //if (ImGui::Button(u8"缩放")) {
    //    x = 15;
    //    y = 15;
    //    size_changed = true;
    //}

    if (size_changed) {
        CrossHairWindow* crosshair_win = dynamic_cast<CrossHairWindow*>(WindowManager::GetWindow("crosshair"));
        crosshair_win->set_size(x, y);
    }
}

void TrayWindow::render_image() {
    ImGui::NewLine();
    ImGui::Text((char*)u8"光标形状");
    ImGui::Separator();

    if (ImGui::Button((char*)u8"新增")) {
        OPENFILENAME opfn;
        ZeroMemory(&opfn, sizeof(OPENFILENAME));
        opfn.lStructSize = sizeof(OPENFILENAME);
        opfn.hwndOwner = hWnd;
        //设置过滤  
        //opfn.lpstrFilter = L"所有文件\0*.*\0文本文件\0*.txt\0MP3文件\0*.mp3\0";
        opfn.lpstrFilter = L"图标文件\0*.png";
        //默认过滤器索引设为1  
        opfn.nFilterIndex = 1;
        //文件名的字段必须先把第一个字符设为 \0  
        WCHAR strFilename[MAX_PATH];//存放文件名
        opfn.lpstrFile = strFilename;
        opfn.lpstrFile[0] = '\0';
        opfn.nMaxFile = sizeof(strFilename);
        //设置标志位，检查目录或文件是否存在  
        opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        //opfn.lpstrInitialDir = NULL;  
        // 显示对话框让用户选择文件  

  
        //初始化  
        if (GetOpenFileName(&opfn))
        {
            ////在文本框中显示文件路径  
            //HWND hEdt = GetDlgItem(hDlg, IDC_EDTFILENAME);
            //SendMessage(hEdt, WM_SETTEXT, NULL, (LPARAM)strFilename);

            std::wcout << "pick up file:" << strFilename << "," << std::filesystem::current_path() << std::endl;

            //char filename[MAX_PATH * 2];
            //WideCharToMultiByte(CP_ACP, 0, strFilename, -1, filename, MAX_PATH, NULL, NULL); //iSize =wcslen(pwsUnicode)+1=6
            
            std::string path = std::filesystem::path(strFilename).string();

            ////std::string path(filename);
            //int index = (int)images.size() + 1;
            //load_texture(path, index, my_texture_srv_gpu_handle, image_width, image_height);
            //images.emplace_back(path, index, my_texture_srv_gpu_handle, image_width, image_height);
            //std::cout << "file name: " << path << std::endl;
        }
    }

    int count = 1;
    for (auto& img : images) {
        //ImGui::Image((ImTextureID)img.my_texture_srv_gpu_handle.ptr, ImVec2((float)img.image_width, (float)img.image_height));
        if (ImGui::ImageButton((ImTextureID)img.my_texture_srv_gpu_handle.ptr, ImVec2(32.0, 32.0))) {
            //std::wstring wpath = to_wstring(img.path);

            std::wstring wpath = std::filesystem::path(img.path).wstring();

            CrossHairWindow* crosshair_win = dynamic_cast<CrossHairWindow*>(WindowManager::GetWindow("crosshair"));
            crosshair_win->set_image(wpath);
        }
        if (count % 5 != 0) {
            ImGui::SameLine();
        }
        count++;
    }
    ImGui::NewLine();
}