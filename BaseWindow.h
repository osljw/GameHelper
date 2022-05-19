#pragma once
#include "framework.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

class BaseWindow
{
public:
	BaseWindow(HINSTANCE hInstance, int nCmdShow, UINT titleID, UINT classID);
	~BaseWindow();

	HINSTANCE get_hinstance() {
		return hInstance;
	}

	HWND get_hwnd() {
		return hWnd;
	}

	//LRESULT WndProcFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool init();
	virtual bool create(int x, int y);
	virtual bool show();

private:
	HINSTANCE hInstance;
	HWND hWnd;
	int nCmdShow;

	WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
	WCHAR szWindowClass[MAX_LOADSTRING];            // ����������


};

