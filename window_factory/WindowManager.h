#pragma once
#include "../GameHelper.h"
#include "BaseWindow.h"

#include <unordered_map>

class WindowManager
{
public:
	static WindowManager& GetWM() {
		static WindowManager wm;
		return wm;
	}

	static void RegisterWindow(std::string name, BaseWindow* win) {
		tlb[name] = win;
	}

	static BaseWindow* GetWindow(std::string name) {
		if (tlb.count(name)) {
			return tlb[name];
		}
		return NULL;
	}


	static std::unordered_map<std::string, BaseWindow*> tlb;
};

