#pragma once

#include "BaseWindow.h"
#include "../framework.h"

#include <comdef.h>
#include <gdiplus.h>
#include <filesystem>

class CrossHairWindow : public BaseWindow
{
public:
	CrossHairWindow(HINSTANCE hInstance);
	~CrossHairWindow();

	Gdiplus::Image* load_image(std::wstring img_path);
	bool show();

	bool update();

	void set_position(float x, float y);
	void set_size(int x, int y);
	void set_image(std::wstring img_path);

private:
	std::filesystem::path workdir;
	Gdiplus::Image* image;
	int center_x;
	int center_y;

	float scale_x = 1.0;
	float scale_y = 1.0;

	int height;
	int width;
};

