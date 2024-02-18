#include "stdafx.hpp"
#include "core.hpp"
#include "init.hpp"
#include "log/log.hpp"
#include "security/security.hpp"
#include "patterns.hpp"
#include "hooks.hpp"
#include "cheat/menu/main.hpp"
#include "util/fiber.hpp"
namespace Core {
	void CCore::Init() {
		Patterns::GetPatterns()->Init();
		Hooks::GetHooks()->Init();

		LOG("Core Init");
	}

	void CCore::Cleanup() {
		//fiber::g_FiberMgr.RemoveAllFibers();
		//Utils::GetFiberManager()->Unload();
		
	}
}