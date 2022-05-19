#pragma once

#include <stdio.h>


class DebugConsole
{
public:
	DebugConsole(bool open);
	~DebugConsole();

	bool debug() {
		return open;
	}

private:
	FILE* pIn;
	FILE* pCout;
	bool open;
};

