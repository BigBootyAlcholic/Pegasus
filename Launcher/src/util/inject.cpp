#include "inject.hpp"
#include <Tlhelp32.h> 
#include <ShlObj.h>
#pragma comment (lib, "shlwapi.lib")

namespace Util::Inject {
	bool Inject(DWORD processId, std::string file, DWORD* errorCode) {
		if (!processId) return false;

		HANDLE proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
		if (!proc) {
			*errorCode = GetLastError();
			return false;
		}

		void* loadLibraryAddress = (void*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		void* remoteStringAddress = (void*)VirtualAllocEx(proc, NULL, file.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(proc, remoteStringAddress, file.c_str(), file.length(), NULL);

		HANDLE thread = CreateRemoteThread(proc, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, remoteStringAddress, NULL, NULL);
		WaitForSingleObject(thread, INFINITE);

		if (VirtualFreeEx(proc, remoteStringAddress, NULL, MEM_RELEASE)) {
			CloseHandle(proc);
		}

		return true;
	}

	DWORD GetProcessIdByName(std::string name) {
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE) return false;

		BOOL loop = Process32First(snapshot, &entry);
		while (loop) {
			if (!strcmp(entry.szExeFile, name.c_str())) {
				return entry.th32ProcessID;
			}

			loop = Process32Next(snapshot, &entry);
		}

		return 0;
	}
}