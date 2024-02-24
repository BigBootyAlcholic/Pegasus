#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/shv/shv.hpp"
namespace Hooks {
	FARPROC GetProcAddressHook(HMODULE moduleHandle, LPCSTR lpProcName) {
		if (moduleHandle == (HMODULE)0x63686970) {
			if (!strcmp(lpProcName, "scriptRegister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptRegister");
				return (FARPROC)&ScriptHook::Proxy::ScriptRegister;
			}

			if (!strcmp(lpProcName, "scriptRegisterAdditionalThread")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptRegisterAdditionalThread");
				return (FARPROC)&ScriptHook::Proxy::ScriptRegisterAdditionalThread;
			}

			if (!strcmp(lpProcName, "scriptUnregister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptUnregister");
				return (FARPROC)&ScriptHook::Proxy::ScriptUnregister;
			}

			if (!strcmp(lpProcName, "nativeInit")) {
				return (FARPROC)&ScriptHook::Proxy::NativeInit;
			}

			if (!strcmp(lpProcName, "nativePush64")) {
				return (FARPROC)&ScriptHook::Proxy::NativePush64;
			}

			if (!strcmp(lpProcName, "nativeCall")) {
				return (FARPROC)&ScriptHook::Proxy::NativeCall;
			}

			if (!strcmp(lpProcName, "getGlobalPtr")) {
				return (FARPROC)&ScriptHook::Proxy::GetGlobalPtr;
			}

			if (!strcmp(lpProcName, "scriptWait")) {
				return (FARPROC)&ScriptHook::Proxy::ScriptWait;
			}

			if (!strcmp(lpProcName, "keyboardHandlerRegister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom keyboardHandlerRegister");
				return (FARPROC)&ScriptHook::Proxy::KeyboardHandlerRegister;
			}

			if (!strcmp(lpProcName, "keyboardHandlerUnregister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom keyboardHandlerUnregister");
				return (FARPROC)&ScriptHook::Proxy::KeyboardHandlerUnregister;
			}

			if (!strcmp(lpProcName, "getScriptHandleBaseAddress")) {
				return (FARPROC)&ScriptHook::Proxy::GetScriptHandleBaseAddress;
			}

			if (!strcmp(lpProcName, "presentCallbackRegister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom presentCallbackRegister");
				return (FARPROC)&ScriptHook::Proxy::PresentCallbackRegister;
			}

			if (!strcmp(lpProcName, "presentCallbackUnregister")) {
				LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom presentCallbackUnregister");
				return (FARPROC)&ScriptHook::Proxy::PresentCallbackUnregister;
			}

			if (!strcmp(lpProcName, "worldGetAllVehicles")) {
				return (FARPROC)&ScriptHook::Proxy::WorldGetAllVehicles;
			}

			if (!strcmp(lpProcName, "worldGetAllPeds")) {
				return (FARPROC)&ScriptHook::Proxy::WorldGetAllPeds;
			}

			if (!strcmp(lpProcName, "worldGetAllObjects")) {
				return (FARPROC)&ScriptHook::Proxy::WorldGetAllObjects;
			}

			if (!strcmp(lpProcName, "worldGetAllPickups")) {
				return (FARPROC)&ScriptHook::Proxy::WorldGetAllPickups;
			}

			LOG_CUSTOM_WARN("ScriptHook", "Failed to verify %s", lpProcName);
		}

		return OgGetProcAddress(moduleHandle, lpProcName);
	}
}