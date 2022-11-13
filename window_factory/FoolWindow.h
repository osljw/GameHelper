#pragma once

#include "ImGuiBaseWindow.h"

#include <string>
#include <vector>
#include <thread>

class FoolWindow : public ImGuiBaseWindow
{
public:
	FoolWindow() {}
	virtual ~FoolWindow() {}

	int render() override;

	int render_timeline();

	int parse_table(std::string& content);
	int render_table();

	bool show_window = true;
	bool show_demo_window = true;
	std::thread task;

	bool table_ready = false;
	std::vector<std::string> table_header;
	std::vector<std::vector<std::string>> table_content;

	int display_count;
};

