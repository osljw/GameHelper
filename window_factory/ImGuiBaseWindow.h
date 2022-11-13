#pragma once
class ImGuiBaseWindow
{
public:
	ImGuiBaseWindow() {}
	virtual ~ImGuiBaseWindow() {}

	virtual int render() { return 0;  }
};

