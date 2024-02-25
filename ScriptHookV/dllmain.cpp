#include "stdafx.hpp"
#include "core/core.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD callReason, LPVOID reserved) {
	switch (callReason) {
	case DLL_PROCESS_ATTACH:
		Global::g_Running = true;
		Global::g_Thread = CreateThread(nullptr, NULL, Thread::Entry, hmod, NULL, nullptr);
		break;
	case DLL_THREAD_DETACH: 
		Global::g_Running = false;
		if (Global::g_Thread)
			CloseHandle(Global::g_Thread);
		break;
	case DLL_PROCESS_DETACH:
		Global::g_Running = false;
		if (Global::g_Thread)
			CloseHandle(Global::g_Thread);
		break;
	}
	return TRUE;
}