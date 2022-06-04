#include "WindowCapture.h"

#include <comdef.h>
#include <gdiplus.h>
#include <gdiplusimagecodec.h>

#include <winrt/Windows.Graphics.Capture.h>

bool WindowCapture::create()
{
    hWnd = CreateWindowEx(
        //WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        WS_EX_APPWINDOW,
        get_winclass().c_str(),
        NULL,
        WS_POPUP,
        //0, 0, 0, 0,
        100, 100, 600, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd)
    {
        std::cout << "CreateWindowEx Failed:" << GetLastError() << std::endl;
        return FALSE;
    }

    return true;
}



bool WindowCapture::show() {
    

    HRESULT hr = S_OK;
    HTHUMBNAIL thumbnail = NULL;

    //HWND src_hWnd = FindWindow(NULL, _T("Steam"));
    HWND src_hWnd = FindWindow(_T("Valve001"), NULL);
    
    std::cout << "find window: " << std::hex << src_hWnd << std::endl;

    // Register the thumbnail
    hr = DwmRegisterThumbnail(hWnd, src_hWnd, &thumbnail);
    if (FAILED(hr)) {
        std::cout << "DwmRegisterThumbnail failed: " << std::hex << hr << std::endl;
        return false;
    }

    std::cout << "DwmRegisterThumbnail success" << std::endl;
    // Specify the destination rectangle size
    //RECT dest = { 0,50,600,400 };
    RECT wndRect;
    ::GetWindowRect(get_hwnd(), &wndRect);

    // Set the thumbnail properties for use
    DWM_THUMBNAIL_PROPERTIES dskThumbProps;
    dskThumbProps.dwFlags = DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE | DWM_TNP_OPACITY | DWM_TNP_RECTDESTINATION;
    dskThumbProps.fSourceClientAreaOnly = FALSE;
    dskThumbProps.fVisible = TRUE;
    dskThumbProps.opacity = 255;
    dskThumbProps.rcDestination = wndRect;

    // Display the thumbnail
    hr = DwmUpdateThumbnailProperties(thumbnail, &dskThumbProps);
    if (SUCCEEDED(hr))
    {
        // ...
        std::cout << "DwmUpdateThumbnailProperties success" << std::endl;
    }

    BaseWindow::show();
    return true;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}


bool WindowCapture::take_photo(std::wstring path) {
    // 窗口大小
    RECT wndRect;
    ::GetWindowRect(get_hwnd(), &wndRect);
    SIZE wndSize = {
        wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top
    };

    HDC hdc = GetDC(NULL);
    HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cy);

    HDC memDC = ::CreateCompatibleDC(hdc);
    SelectObject(memDC, memBitmap);

    BitBlt(memDC, 0, 0, wndSize.cx, wndSize.cy, hdc, 0, 0, SRCCOPY);

    //保存成文件  
    {
        //L"image/bmp" L"image/jpeg"  L"image/gif" L"image/tiff" L"image/png"  
        CLSID pngClsid;
        GetEncoderClsid(L"image/bmp", &pngClsid);

        Gdiplus::Bitmap* pbmSrc = Gdiplus::Bitmap::FromHBITMAP(memBitmap, NULL);
        //L"C:\\test.bmp"
        pbmSrc->Save(path.data(), &pngClsid);
        delete pbmSrc;
    }

    //清理工作  
    SelectObject(memDC, (HBITMAP)NULL);
    DeleteDC(memDC);
    DeleteObject(memBitmap);

    return true;
}