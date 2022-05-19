#pragma once
#include "../framework.h"
#include "../Resource.h"

class BaseWindow
{
public:
	// �޽��洰��
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

	// ע�ᴰ������
	virtual bool init();

	// ��������
	virtual bool create();

	// ��ʾ����
	virtual bool show();

	// ��Ⱦ����
	virtual bool render() { return true; };

public:
	HINSTANCE hInstance;
	HWND hWnd;

private:
	std::wstring winClassType = L"";	// ��������
};

