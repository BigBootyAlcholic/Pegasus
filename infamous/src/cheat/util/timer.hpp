#pragma once
#include "stdafx.hpp"

namespace Menu::Timers {
	void RunTimedFunction(int* Timer, int Ms, std::function<void()> Callback);
}