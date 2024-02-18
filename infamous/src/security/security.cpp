#include "stdafx.hpp"
#include "security.hpp"
#include "checks/debuggercheck.hpp"

namespace Security {

	void Init() {
		g_Running = true;
		Checks::Debugger::Init();
	}

	bool Run() {
		Checks::Debugger::Run();
		return g_Running;
	}
}