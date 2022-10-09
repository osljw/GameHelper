// GameHelper.cpp : 定义应用程序的入口点。
//

//#include "coroutine_test.h"

#include "GameHelper.h"
#include "App.h"
#include "Tray.h"
#include "ResourceManager.h"
#include "DebugConsole.h"
//#include "imgui_init.h"
#include "window_factory/BaseWindow.h"
#include "window_factory/CrossHairWindow.h"
#include "window_factory/TrayWindow.h"
#include "window_factory/WindowManager.h"
#include "DesktopCapture.h"
//#include "WindowCapture.h"



#include "WGC.h"



//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "windowsapp")

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DebugConsole dc(true);

    App app(hInstance);

    //BaseWindow win(hInstance);
    //CrossHairWindow crosshair_win(hInstance);

    //bool success;
    //success = crosshair_win.init();
    //if (!success) {
    //    std::cout << "window init failed" << std::endl;
    //    return -1;
    //}
    //success = crosshair_win.create();
    //if (!success) {
    //    std::cout << "window create failed" << std::endl;
    //    return -1;
    //}
    //crosshair_win.show();


    //TrayWindow tray_win(hInstance);
    //success = tray_win.init();
    //if (!success) {
    //    std::cout << "TrayWindow init failed" << std::endl;
    //    return -1;
    //}
    //success = tray_win.create();
    //if (!success) {
    //    std::cout << "TrayWindow create failed" << std::endl;
    //    return -1;
    //}
    //tray_win.show();

    //WindowManager::RegisterWindow("crosshair", &crosshair_win);
    //WindowManager::RegisterWindow("tray", &tray_win);


    //WindowCapture wc(hInstance);
    //wc.init();
    //wc.create();
    //wc.show();

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEHELPER));

    // imgui 绑定到托盘窗口
    //ImGuiD3D11 imgui_manager(tray_win.get_hwnd());
    //imgui_manager.Init();

    //tray_win.load();

    std::cout << "main======= thread id: " << std::this_thread::get_id() << std::endl;

    //DesktopCapture desk_cap;
    //WGC wgc;
    //HWND src_hWnd = FindWindow(NULL, _T("Steam"));
    //wgc.init(src_hWnd);


    int count = 0;
    // Main loop
    bool done = false;
    while (!done)
    {
        
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            //std::cout << "all ======== msg: " << msg.message << ", " << std::hex << msg.message << std::endl;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        //MSG msg = {};
        //while (GetMessageW(&msg, nullptr, 0, 0))
        //{
        //    TranslateMessage(&msg);
        //    DispatchMessageW(&msg);
        //}

        

        count++;
        //if (count % 10000 == 0) wc.take_photo(L"E:\\workspace\\ce\\test.bmp");

        //std::cout << "========= imgui_render ========= " << done << std::endl;

        app.Render();

        // Start the Dear ImGui frame
        //imgui_manager.Begin();
        //

        //tray_win.render();
        ////desk_cap.render();
        //wgc.render();

        //imgui_manager.End();
    }

    //imgui_manager.Destroy();


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
