#include "stdafx.hpp"
#include "timer.hpp"
#include "rage/invoker/natives.hpp"

namespace Menu::Timers {
	void RunTimedFunction(int* Timer, int Ms, std::function<void()> Callback) {
		if (*Timer < Native::GetGameTimer()) {
			*Timer = Native::GetGameTimer() + Ms;
			Callback();
		}
	}
}