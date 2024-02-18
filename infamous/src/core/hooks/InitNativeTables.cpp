#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
namespace Hooks {

	

	bool InitNativeTablesHook(Rage::scrProgram* _this) {
		bool _return = OgInitNativeTablesHook(_this);
	//	Menu::g_ScriptPatcher->OnScriptLoad(_this);
		

		return _return;
	}
}