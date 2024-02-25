#pragma once
#include "stdafx.hpp"

namespace Util::Process {
	bool IsRunning(std::string processName);
	bool Create(std::string path);
	bool ExecuteCommand(std::string);
}