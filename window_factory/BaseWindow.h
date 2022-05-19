#pragma once
#include "../framework.h"
#include "../Resource.h"

class BaseWindow
{
public:
	// 无界面窗口
	BaseWindow(HINSTANCE hInstance);
	BaseWindow(HINSTANCE hInstance, UINT classID);
	BaseWindow(HINSTANCE hInstance, std::wstring winClassType);
	virtual ~BaseWindow();

	HINSTANCE get_hinstance() {
		return hInstance;
	}

	HWND get_hwnd() {
		return hWnd;
	}

	std::wstring get_winclass() {
		return winClassType;
	}

	// 注册窗口类型
	virtual bool init();

	// 创建窗口
	virtual bool create();

	// 显示窗口
	virtual bool show();

	// 渲染窗口
	virtual bool render() { return true; };

public:
	HINSTANCE hInstance;
	HWND hWnd;

private:
	std::wstring winClassType = L"";	// 窗口类型
};

