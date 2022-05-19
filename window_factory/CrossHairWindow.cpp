#include "CrossHairWindow.h"


#pragma comment(lib, "gdiplus.lib")


ULONG_PTR gdiplusStartupToken;

CrossHairWindow::CrossHairWindow(HINSTANCE hInstance) : BaseWindow(hInstance){
    Gdiplus::GdiplusStartupInput gdiInput;
    Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, NULL);
}

CrossHairWindow::~CrossHairWindow() {
    Gdiplus::GdiplusShutdown(gdiplusStartupToken);
}

Gdiplus::Image* CrossHairWindow::load_image(std::wstring img_path) {
    // 加载crsoss hair 图片;
     Gdiplus::Image* imageptr = Gdiplus::Image::FromFile(img_path.c_str());
    return imageptr;
}

bool CrossHairWindow::show() {
    std::wstring img_path = L"resource\\crosshair.png";

    image = load_image(img_path);
    if (image == nullptr) {
        std::wcout << "load image failed, img path: " << img_path << std::endl;
        return false;
    }

    int height = image->GetHeight();
    int width = image->GetWidth();
    int left = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int top = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    SetWindowPos(get_hwnd(), HWND_TOPMOST, left, top, width, height, SWP_SHOWWINDOW);

    //// 窗口大小
    RECT wndRect;
    ::GetWindowRect(get_hwnd(), &wndRect);
    SIZE wndSize = {
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top
    };

    HDC hdc = GetDC(NULL);
    //HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cy);
    HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, width, height);

    HDC memDC = ::CreateCompatibleDC(NULL);
    HGDIOBJ original = SelectObject(memDC, memBitmap);

    Gdiplus::Graphics graphics(memDC);
    //graphics.DrawImage(image, 0, 0, wndSize.cx, wndSize.cy);
    graphics.DrawImage(image, 0, 0, width, height);

    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;


    POINT srcPos = {left, top};
    //SIZE sizeWnd = { wndSize.cx, wndSize.cy };
    SIZE sizeWnd = { width, height };
    POINT dstPos = { 0, 0 };

    //bool ret = UpdateLayeredWindow(hWnd, hdc, &ptLocation, &szWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
    bool ret = UpdateLayeredWindow(get_hwnd(), hdc, &srcPos, &sizeWnd, memDC, &dstPos, 0, &blend, ULW_ALPHA);
    if (!ret) {
        std::cout << "UpdateLayeredWindow failed" << std::endl;
        return ret;
    }

    return true;
}

void CrossHairWindow::set_position(float x, float y)
{
    int height = image->GetHeight();
    int width = image->GetWidth();
    int left = GetSystemMetrics(SM_CXSCREEN) * x - width/2;
    int top = GetSystemMetrics(SM_CYSCREEN) * y - height/2;

    SetWindowPos(get_hwnd(), HWND_TOPMOST, left, top, width, height, SWP_SHOWWINDOW);
}
