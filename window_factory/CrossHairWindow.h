#pragma once

#include "BaseWindow.h"
#include "../framework.h"

#include <comdef.h>
#include <gdiplus.h>

class CrossHairWindow : public BaseWindow
{
public:
	CrossHairWindow(HINSTANCE hInstance);
	~CrossHairWindow();

	Gdiplus::Image* load_image(std::wstring img_path);
	bool show();

	void set_position(float x, float y);

private:
	Gdiplus::Image* image;
};

