#include "BaseWindow.h"
#include "imgui_init.h"
#include "Tray.h"


#include <functional>
#include <unordered_map>

#include <gdiplus.h> 
#pragma comment(lib, "gdiplus.lib") 

ULONG_PTR gdiplusStartupToken;
Gdiplus::Image* image;

BaseWindow::BaseWindow(HINSTANCE hInstance, int nCmdShow, UINT titleID, UINT classID): hInstance(hInstance), nCmdShow(nCmdShow) {
    LoadStringW(hInstance, titleID, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, classID, szWindowClass, MAX_LOADSTRING);
}

BaseWindow::~BaseWindow() {
    Gdiplus::GdiplusShutdown(gdiplusStartupToken);
    CleanupDeviceD3D();
}


class MessageFunc {
public:
    void operator () () {
        std::cout << "func called" << std::endl;
    }
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

std::unordered_map<UINT, MessageFunc> message_manager = {
    {WM_TRAY, MessageFunc()}
};
std::unordered_map<UINT, MessageFunc> command_manager;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message) {
        case WM_COMMAND: {
            UINT wmId = LOWORD(wParam);
            std::cout << "WM_COMMAND: " << wmId << std::endl;

            if (command_manager.count(wmId)) {
                command_manager[wmId]();
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_GRAYTEXT + 1));
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    if (message_manager.count(message)) {
        message_manager[message]();
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


HMENU hMenu;
LRESULT WndProcFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    std::cout << std::endl;
    std::cout << "msg: " << message << std::endl;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    std::cout << "after msg: " << message << ", lparam: " << lParam << std::endl;

    switch (message)
    {
        case WM_TRAY:
        {
            int wmId = LOWORD(lParam);
            switch (wmId) {
                std::cout << "tray cmd:" << wmId << std::endl;
                case WM_RBUTTONDOWN:
                {
                    std::cout << "tray right click: " << std::endl;
                    POINT pt;
                    GetCursorPos(&pt);
                    
                    hMenu = CreatePopupMenu();

                    if (IsWindowVisible(hWnd)) {
                        AppendMenu(hMenu, MF_STRING, 1234, TEXT("隐藏"));
                    } else {
                        AppendMenu(hMenu, MF_STRING, 1234, TEXT("显示"));
                    }
                    
                    //AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                    AppendMenu(hMenu, MF_STRING, 2, TEXT("退出"));
                    //AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                    //AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                    // note:	must set window to the foreground or the menu won't disappear when it should
                    SetForegroundWindow(hWnd);
                    TrackPopupMenuEx(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, hWnd, NULL);
                    
                    break;
                }
                default: {
                    std::cout << "tray other:" << wmId << std::endl;
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
             }
        }
        break;
        //case WM_ERASEBKGND:
        //{
        //    std::cout << "=========WM_ERASEBKGND: " << message << std::endl;
        //    break;
        //}
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            std::cout << "=========WM_COMMAND: " << wmId << std::endl;
            // 分析菜单选择:
            switch (wmId)
            {
            case 1234:
            {
                std::cout << std::endl;
                std::cout << "msg123456: =======" << std::endl;
                if (IsWindowVisible(hWnd)) {
                    ShowWindow(hWnd, SW_HIDE);
                } else {
                    ShowWindow(hWnd, SW_SHOW);
                }
                break;
            }
            case IDM_ABOUT:
                //DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            std::cout << "=========WM_PAINT: " << std::endl;

            if (image == nullptr) break;


            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            //// TODO: 在此处添加使用 hdc 的任何绘图代码...

            //RECT wndRect;
            //::GetWindowRect(hWnd, &wndRect);
            //SIZE wndSize = {
            //    wndRect.right - wndRect.left,
            //    wndRect.bottom - wndRect.top
            //};
            //HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cy);

            //HDC memDC = ::CreateCompatibleDC(NULL);
            //SelectObject(memDC, memBitmap);

            //Gdiplus::Graphics graphics(memDC);
            ////graphics.DrawImage(image, 200, 200, wndSize.cx, wndSize.cy);
            //graphics.DrawImage(image, 0, 0, wndSize.cx, wndSize.cy);
            //BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memDC, 0, 0, SRCCOPY);

            //EndPaint(hWnd, &ps);

            //InvalidateRect(hWnd, NULL, FALSE);

        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool BaseWindow::init() {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    //std::function<LRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM)> func = std::bind(&BaseWindow::WndProc, *this);
    //wcex.lpfnWndProc = WndProc;
    wcex.lpfnWndProc = WndProcFunc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEHELPER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = 0;
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEHELPER);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    ATOM ret = RegisterClassExW(&wcex);

    if (ret == 0) {
        std::cout << "register class failed" << std::endl;
    }

    Gdiplus::GdiplusStartupInput gdiInput;
    Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, NULL);
    
    return true;
}


bool BaseWindow::show() {


    HDC hdc = GetDC(NULL);
    RECT wndRect;
    ::GetWindowRect(hWnd, &wndRect);
    SIZE wndSize = {
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top
    };
    HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cy);

    HDC memDC = ::CreateCompatibleDC(NULL);
    HGDIOBJ original = SelectObject(memDC, memBitmap);

    Gdiplus::Graphics graphics(memDC);
    //graphics.DrawImage(image, 200, 200, wndSize.cx, wndSize.cy);
    graphics.DrawImage(image, 0, 0, wndSize.cx, wndSize.cy);
    //BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memDC, 0, 0, SRCCOPY);


    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;
    //POINT ptPos = { wndRect.left, wndRect.top };
    //POINT ptPos = { , 0 };
    SIZE sizeWnd = { wndSize.cx, wndSize.cy };
    POINT ptSrc = { 0, 0 };
    std::cout << "show window" << std::endl;
    //bool ret = UpdateLayeredWindow(hWnd, hdc, &ptLocation, &szWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
    bool ret = UpdateLayeredWindow(hWnd, hdc, NULL, &sizeWnd, memDC, &ptSrc, 0, &blend, ULW_ALPHA);
    if (!ret) {
        std::cout << "UpdateLayeredWindow failed" << std::endl;
        return ret;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //SetLayeredWindowAttributes(
    //    hWnd,
    //    0, // no color key
    //    180, // alpha value
    //    LWA_ALPHA);
    

    // 透明窗口
    //SetLayeredWindowAttributes(hWnd, RGB(50, 0, 0), 10, LWA_ALPHA);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);


    
    return true;
}

bool BaseWindow::create(int x, int y) {
    //Gdiplus::Image image(L"crosshair.png");
    image = Gdiplus::Image::FromFile(L"crosshair.png");
    if (image == nullptr) {
        std::cout << "load image failed" << std::endl;
        return false;
    }
    UINT height = image->GetHeight();
    UINT width = image->GetWidth();

    std::cout << "desk x: " << GetSystemMetrics(SM_CXSCREEN) << "desk y:" << GetSystemMetrics(SM_CYSCREEN) << std::endl;

    //hWnd = CreateWindowW(
    //    szWindowClass, 
    //    szTitle, 
    //    //WS_OVERLAPPED | WS_POPUP,
    //    WS_POPUP | WS_MAXIMIZE,
    //    CW_USEDEFAULT, 0, 
    //    //200, 200,
    //    CW_USEDEFAULT, 0, 
    //    //600, 400,
    //    nullptr, nullptr, hInstance, nullptr);


    // WS_EX_TOPMOST（窗口置顶）， WS_EX_LAYERED（窗口透明需要）
    hWnd = CreateWindowEx(
        //WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
        //WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        //WS_EX_TOPMOST | WS_EX_COMPOSITED,
        //WS_EX_LAYERED,
        szWindowClass,
        NULL,
        //szTitle,
        WS_POPUP,
        //0, 0, 0, 0,
        //200, 200,600, 400,
        //0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        //200, 200, height, width,
        //GetSystemMetrics(SM_CXSCREEN)/2 - width/2, GetSystemMetrics(SM_CYSCREEN)/2 - height/2, width, height,
        x, y, width, height,
        nullptr, nullptr, hInstance, nullptr
    );

    std::cout << "CreateWindowEx:" << std::endl;

    if (!hWnd)
    {
        std::cout << "CreateWindowEx Failed:" << GetLastError() << std::endl;
        return FALSE;
    }

    if (!CreateDeviceD3D(hWnd))
    {
        std::cout << "CreateDeviceD3D failed: " << GetLastError() << std::endl;
        CleanupDeviceD3D();
        //::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    std::cout << "imgui init:" << std::endl;
    //imgui_init(hWnd);




    // 不规则窗口
    //SetWindowRgn(hWnd, HRGN(), true);

    // 透明窗口
    //SetLayeredWindowAttributes(hWnd, RGB(50, 0, 0), 10, LWA_ALPHA);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);


    return TRUE;
}

