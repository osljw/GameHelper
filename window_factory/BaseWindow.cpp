#include "BaseWindow.h"
#include "../imgui_init.h"
#include "../Tray.h"
#include "WindowManager.h"
#include "TrayWindow.h"


#include <functional>
#include <unordered_map>


BaseWindow::BaseWindow(HINSTANCE hInstance) {
    winClassType = L"default_window_class##";
}

BaseWindow::BaseWindow(HINSTANCE hInstance, UINT classID): hInstance(hInstance) {
    LPWSTR pStr = NULL;
    LoadStringW(hInstance, classID, pStr, 0);

    winClassType = std::wstring(pStr);
}

BaseWindow::BaseWindow(HINSTANCE hInstance, std::wstring winClassType) : hInstance(hInstance), winClassType(winClassType) {

}

BaseWindow::~BaseWindow() {
    //CleanupDeviceD3D();
}


//class MessageFunc {
//public:
//    void operator () () {
//        std::cout << "func called" << std::endl;
//    }
//};
//
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//std::unordered_map<UINT, MessageFunc> message_manager = {
//    {WM_TRAY, MessageFunc()}
//};
//std::unordered_map<UINT, MessageFunc> command_manager;
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//
//    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//        return true;
//
//    switch (message) {
//        case WM_COMMAND: {
//            UINT wmId = LOWORD(wParam);
//            std::cout << "WM_COMMAND: " << wmId << std::endl;
//
//            if (command_manager.count(wmId)) {
//                command_manager[wmId]();
//            }
//
//            return DefWindowProc(hWnd, message, wParam, lParam);
//        }
//        case WM_PAINT: {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: 在此处添加使用 hdc 的任何绘图代码...
//            //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_GRAYTEXT + 1));
//            EndPaint(hWnd, &ps);
//            break;
//        }
//        case WM_DESTROY:
//            PostQuitMessage(0);
//            break;
//        default:
//            break;
//    }
//
//    if (message_manager.count(message)) {
//        message_manager[message]();
//    }
//
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT BaseWindownWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    //std::cout << std::endl;
    std::cout << "msg: " << message << std::endl;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    //std::cout << "after msg: " << message << ", lparam: " << lParam << std::endl;
    

    switch (message)
    {
        //case WM_ACTIVATEAPP: {
        //    std::cout << "message WM_ACTIVATE: " << WM_ACTIVATE << std::endl;

        //    // 窗口正在停用
        //    if (wParam == FALSE) {
        //        TrayWindow* tray_win = dynamic_cast<TrayWindow*>(WindowManager::GetWindow("tray"));
        //        if (tray_win == nullptr) return 0;
        //        tray_win->show_tray_menu = false;
        //    }
        //    break;
        //}
        case WM_TRAY: {
            //std::cout << "WM_TRAY" << std::endl;

            int wmId = LOWORD(lParam);
            switch (wmId) {
                //std::cout << "tray cmd:" << wmId << std::endl;
                case WM_RBUTTONDOWN:
                {

                    POINT pt;
                    GetCursorPos(&pt);
                    //TrayWindow* tray_win = dynamic_cast<TrayWindow*>(WindowManager::GetWindow("tray"));
                    //tray_win->show_tray_menu = true;
                    //tray_win->tray_menu_pos = ImVec2((float)pt.x, (float)pt.y);

                    
                    HWND tray_hwnd = WindowManager::GetWindow("tray")->get_hwnd();
                    HWND crosshair_hwnd = WindowManager::GetWindow("crosshair")->get_hwnd();
                    HMENU hMenu = CreatePopupMenu();

                    AppendMenu(hMenu, MF_STRING, 1235, TEXT("设置"));
                    if (IsWindowVisible(crosshair_hwnd)) {
                        AppendMenu(hMenu, MF_STRING, 1234, TEXT("隐藏"));
                    } else {
                        AppendMenu(hMenu, MF_STRING, 1234, TEXT("显示"));
                    }
                    AppendMenu(hMenu, MF_STRING, 1236, TEXT("退出"));

                    // note:	must set window to the foreground or the menu won't disappear when it should
                    SetForegroundWindow(tray_hwnd);
                    TrackPopupMenuEx(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, tray_hwnd, NULL);
                    
                    break;
                }
                default: {
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
             }
            break;
        }
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId) {
                case 1234: {
                    HWND crosshair_hwnd = WindowManager::GetWindow("crosshair")->get_hwnd();

                    std::cout << "crosshair_hwnd visiable: " << IsWindowVisible(crosshair_hwnd) << std::endl;
                    if (IsWindowVisible(crosshair_hwnd)) {
                        ShowWindow(crosshair_hwnd, SW_HIDE);
                    } else {
                        ShowWindow(crosshair_hwnd, SW_SHOW);
                    }
                    break;
                }
                case 1235: {
                    
                    TrayWindow* tray_win = dynamic_cast<TrayWindow*>(WindowManager::GetWindow("tray"));
                    tray_win->show_main_window = !tray_win->show_main_window;
                    std::cout << "main window msg:" << 1235 << "," << tray_win->show_main_window << std::endl;
                    
                    break;
                }
                //case IDM_ABOUT:
                //    //DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                //    break;
                case 1236: {
                    HWND tray_hwnd = WindowManager::GetWindow("tray")->get_hwnd();
                    HWND crosshair_hwnd = WindowManager::GetWindow("crosshair")->get_hwnd();

                    DestroyWindow(tray_hwnd);
                    DestroyWindow(crosshair_hwnd);
                    break;
                }
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT: {
            std::cout << "WM_PAINT" << std::endl;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            EndPaint(hWnd, &ps);
            break;
        }
        case WM_CLOSE: {
            TrayWindow* tray_win = dynamic_cast<TrayWindow*>(WindowManager::GetWindow("tray"));
            Shell_NotifyIcon(NIM_DELETE, &tray_win->get_nid());

            std::cout << "MSG: WM_CLOSE" << std::endl;
            HWND tray_hwnd = WindowManager::GetWindow("tray")->get_hwnd();
            HWND crosshair_hwnd = WindowManager::GetWindow("crosshair")->get_hwnd();

            DestroyWindow(crosshair_hwnd);
            DestroyWindow(tray_hwnd);


            break;
        }
        case WM_DESTROY: {
            std::cout << "MSG: WM_DESTROY" << std::endl;
            TrayWindow* tray_win = dynamic_cast<TrayWindow*>(WindowManager::GetWindow("tray"));
            Shell_NotifyIcon(NIM_DELETE, &tray_win->get_nid());
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool BaseWindow::init() {
    WNDCLASSEXW wcex;
    if (GetClassInfoEx(hInstance, winClassType.c_str(), &wcex)) {
        std::cout << "get win class failed: " << std::endl;
        return true;
    }

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = BaseWindownWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEHELPER));
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = 0; // 透明窗口
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEHELPER);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = winClassType.c_str();
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TRAY));

    ATOM ret = RegisterClassExW(&wcex);

    if (ret == 0) {
        std::cout << "register class failed" << std::endl;
    }

    
    return true;
}




bool BaseWindow::create() {

    hWnd = CreateWindowEx(
        //WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
        winClassType.c_str(),
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


    // 不规则窗口
    //SetWindowRgn(hWnd, HRGN(), true);

    // 透明窗口
    //SetLayeredWindowAttributes(hWnd, RGB(50, 0, 0), 10, LWA_ALPHA);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);


    return TRUE;
}

bool BaseWindow::show() {

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // 窗口整体透明度设置
    //SetLayeredWindowAttributes(hWnd, RGB(50, 0, 0), 10, LWA_ALPHA);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

    return true;
}