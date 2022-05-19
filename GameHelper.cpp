// GameHelper.cpp : 定义应用程序的入口点。
//


#include "GameHelper.h"
#include "Tray.h"
#include "ResourceManager.h"
#include "DebugConsole.h"
#include "imgui_init.h"
#include "window_factory/BaseWindow.h"
#include "window_factory/CrossHairWindow.h"
#include "window_factory/TrayWindow.h"
#include "window_factory/WindowManager.h"

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //DebugConsole dc(true);

    //WCHAR* winclass = ResourceManager::GetString(IDC_GAMEHELPER);

    //BaseWindow win(hInstance);
    CrossHairWindow crosshair_win(hInstance);

    bool success;
    success = crosshair_win.init();
    if (!success) {
        std::cout << "window init failed" << std::endl;
        return -1;
    }
    success = crosshair_win.create();
    if (!success) {
        std::cout << "window create failed" << std::endl;
        return -1;
    }
    crosshair_win.show();

    //BaseWindow win2(hInstance, nCmdShow, IDS_APP_TITLE, IDC_GAMEHELPER);
    //win2.init();
    //win2.create(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
    //win2.show();

    TrayWindow tray_win(hInstance);
    success = tray_win.init();
    if (!success) {
        std::cout << "TrayWindow init failed" << std::endl;
        return -1;
    }
    success = tray_win.create();
    if (!success) {
        std::cout << "TrayWindow create failed" << std::endl;
        return -1;
    }
    tray_win.show();

    WindowManager::RegisterWindow("crosshair", &crosshair_win);
    WindowManager::RegisterWindow("tray", &tray_win);

    //HWND hWnd = tray_win.get_hwnd();
    //Tray tray(hInstance, hWnd);

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEHELPER));

    // imgui 绑定到托盘窗口
    imgui_init(tray_win.get_hwnd());
    tray_win.load();

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            //std::cout << "msg: " << msg.message << std::endl;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        //std::cout << "========= imgui_render ========= " << done << std::endl;

        // Start the Dear ImGui frame
        //imgui_render();
        ImguiRender::Begin();

        tray_win.render();

        ImguiRender::End();
    }

    imgui_destroy();


    return 0;
}


// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
