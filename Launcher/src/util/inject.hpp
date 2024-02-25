#pragma once
#include "stdafx.hpp"

namespace Util::Inject {
	bool Inject(DWORD processId, std::string file, DWORD* errorCode);
	DWORD GetProcessIdByName(std::string name);
}