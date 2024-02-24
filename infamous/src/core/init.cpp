#include "stdafx.hpp"
#include "init.hpp"
#include "core.hpp"
#include "log/log.hpp"
#include "util/hooking/hooking.hpp"
#include "cheat/util/script/script_patcher_service.hpp"
#include "framework/d3d.hpp"
#include "core/patterns.hpp"
#include "util/fiber.hpp"
#include "cheat/util/shop_manager.hpp"
#include "exceptions/exception.hpp"
#include "util/config.hpp"
#include "cheat/util/matchmaking_manager.hpp"
namespace Core::Init {

	/*main program thread*/
	DWORD WINAPI Start(LPVOID handle) {
		Core::Log::Load();
		Exceptions::initExceptionHandler();
		Utils::GetConfig()->Initialize();
		LOG("Pegasus Started");

		if (!(Core::g_GameWindow = FindWindowA(("grcWindow"), NULL))) {
			int timeout = 0;
			while (!Core::g_GameWindow) {
				if (timeout >= 20) {
					LOG_ERROR(("Failed to find game window"));
					Cleanup(handle);
					return 0;
				}

				Core::g_GameWindow = FindWindowA(("grcWindow"), NULL);
				timeout++;
				Sleep(1000);
			}
		}

		Core::g_Running = true;

		//LOG_SUCCESS("Pegasus Running");

		MODULEINFO ModuleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &ModuleInfo, sizeof(ModuleInfo));

		Core::g_GameAddress = (uint64_t)ModuleInfo.lpBaseOfDll;
		Core::g_GameSize = (uint64_t)ModuleInfo.SizeOfImage;

		LOG("Game Base: %llx", Core::g_GameAddress);
		LOG("Game Size: %llx", Core::g_GameSize);

		//LOG_SUCCESS("Created Script Patches");

		auto ScriptPatcherInstance = std::make_unique<Menu::ScriptPatcher>();
		GetCore()->Init();

		auto MatchMakingManager = std::make_unique<Menu::MatchmakingManager>();

		d3d::initialize();

	//	LOG_SUCCESS("Custom Textures Loaded");

		auto g_NativeHooks = std::make_unique<Hooking::NativeHooks>();
		//LOG_SUCCESS("Created Script Hooks");

		while (g_Running) {

			if (GetAsyncKeyState(VK_DELETE)) {
				g_Running = false;
			}
		}
		g_NativeHooks.reset();
		MatchMakingManager.reset();
		ScriptPatcherInstance.reset();
		Cleanup(handle);
	}

	DWORD WINAPI Cleanup(LPVOID handle) {
		Core::GetCore()->Cleanup();
		Hooking::GetHooking()->Unload();
		Core::Log::Cleanup();
		FreeLibraryAndExitThread(static_cast<HMODULE>(handle), 0);
	}
}