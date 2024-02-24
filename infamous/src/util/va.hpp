#pragma once
#include "stdafx.hpp"

namespace Utils::VA {
	const char* VA(const char* Format, ...);
	const char* VAP(char* Buffer, const char* Format, ...);
	const wchar_t* VAW(const char* Format, ...);
	void Cleanup();
}