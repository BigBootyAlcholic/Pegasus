#include "stdafx.hpp"
#include "../hooks.hpp"

namespace Hooks {

	std::string GetStackTrace() {
		std::string s("[");

		typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
		CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));

		if (func != NULL) {
			HMODULE hMods[1024];
			DWORD cbNeeded;
			EnumProcessModules((HANDLE)-1, hMods, sizeof(hMods), &cbNeeded);
			const int kMaxCallers = 62;
			void* callers[kMaxCallers];
			int count = (func)(0, kMaxCallers, callers, NULL);
			for (int i = 1; i < count; i++) {
				DWORD64 callerAddr = (DWORD64)callers[i];
				HMODULE currentCallerModule = NULL;

				for (unsigned int i2 = 0; i2 < (cbNeeded / sizeof(HMODULE)); i2++) {
					MODULEINFO information = { 0 };
					GetModuleInformation((HANDLE)-1, hMods[i2], &information, sizeof(information));
					if (callerAddr >= (DWORD64)information.lpBaseOfDll && callerAddr <= (DWORD64)information.lpBaseOfDll + information.SizeOfImage) {
						currentCallerModule = hMods[i2];
						break;
					}
				}

				if (currentCallerModule != NULL) {
					char moduleName[MAX_PATH];
					GetModuleBaseNameA((HANDLE)-1, currentCallerModule, moduleName, MAX_PATH);
					char buf[512];
					sprintf_s(buf, "0x%llx [%s]%s", callerAddr - (DWORD64)currentCallerModule, moduleName, i != count - 1 ? ", " : "");
					s += buf;
				}
				else {
					char buf[512];
					sprintf_s(buf, "0x%llx %s", callerAddr, i != count - 1 ? ", " : "");
					s += buf;
				}
			}

			s += "]";
			return s;
		}

		return "Unk";
	}

	void MessageBoxTerminationHook(uint32_t Ecx, uint32_t Edx) {

		if (Ecx == Joaat("ERR_NET_EVENT")) {
			return;
		}

		LOG("MessageBoxTermination [%x][%x] %s", Ecx, Edx, GetStackTrace().c_str());
	}
}