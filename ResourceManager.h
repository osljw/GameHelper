#pragma once
#include "framework.h"

class ResourceManager
{
public:
	static ResourceManager& GetRM() {
		static ResourceManager rm;
		return rm;
	}

	static WCHAR* GetString(UINT resource_id) {
		WCHAR* pStr = NULL;
		HINSTANCE hInstance = GetModuleHandle(NULL);
		LoadStringW(hInstance, resource_id, pStr, 0);
		return pStr;
	}


};

