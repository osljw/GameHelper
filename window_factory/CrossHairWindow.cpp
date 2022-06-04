#include "CrossHairWindow.h"

#include <filesystem>

#pragma comment(lib, "gdiplus.lib")


ULONG_PTR gdiplusStartupToken;

CrossHairWindow::CrossHairWindow(HINSTANCE hInstance) : BaseWindow(hInstance){
    Gdiplus::GdiplusStartupInput gdiInput;
    Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, NULL);

    workdir = std::filesystem::current_path();
}

CrossHairWindow::~CrossHairWindow() {
    Gdiplus::GdiplusShutdown(gdiplusStartupToken);
}

BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

Gdiplus::Image* CrossHairWindow::load_image(std::wstring img_path) {
    // 加载crsoss hair 图片;

    std::filesystem::current_path(workdir);
    image = Gdiplus::Image::FromFile(img_path.c_str());
    std::wcout << img_path << "," << FileExists(img_path.c_str()) << std::filesystem::current_path() << " img width:" << image->GetHeight() << ", " << image->GetWidth() << std::endl;
    if (image == nullptr) {
        std::wcout << "load image failed, img path: " << img_path << std::endl;
        return nullptr;
    }

    return image;
}

bool CrossHairWindow::show() {
    std::wstring img_path = L"resource\\crosshair.png";

    load_image(img_path);

    width = image->GetWidth();
    height = image->GetHeight();
    center_x = GetSystemMetrics(SM_CXSCREEN) / 2;
    center_y = GetSystemMetrics(SM_CYSCREEN) / 2;

    update();

    return true;
}

bool CrossHairWindow::update() {
    //int height = image->GetHeight() * scale_x;
    //int width = image->GetWidth() * scale_y;
    int left = center_x - width / 2;
    int top = center_y - height / 2;

    SetWindowPos(get_hwnd(), HWND_TOPMOST, left, top, width, height, SWP_SHOWWINDOW);

    // 窗口大小
    RECT wndRect;
    ::GetWindowRect(get_hwnd(), &wndRect);
    SIZE wndSize = {
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top
    };

    HDC hdc = GetDC(NULL);
    HDC memDC = ::CreateCompatibleDC(NULL);
    HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, width, height);

    HGDIOBJ original = SelectObject(memDC, memBitmap);

    Gdiplus::Graphics graphics(memDC);
    graphics.DrawImage(image, 0, 0, width, height);

    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    POINT srcPos = { left, top };
    SIZE sizeWnd = { width, height };
    POINT dstPos = { 0, 0 };

    // memDC -> hdc
    bool ret = UpdateLayeredWindow(get_hwnd(), hdc, &srcPos, &sizeWnd, memDC, &dstPos, 0, &blend, ULW_ALPHA);
    if (!ret) {
        std::cout << "UpdateLayeredWindow failed: " << width << "," << height << std::endl;
        return ret;
    }

    return true;
}

void CrossHairWindow::set_position(float x, float y)
{
    center_x = GetSystemMetrics(SM_CXSCREEN) * x;
    center_y = GetSystemMetrics(SM_CYSCREEN) * y;

    update();
}

void CrossHairWindow::set_size(int x, int y)
{
    width = x;
    height = y;

    update();
}

void CrossHairWindow::set_image(std::wstring img_path)
{
    if (image) {
        delete image;
    }
    std::wcout << "load img:" << img_path << std::endl;
    load_image(img_path);
    update();
}
