#include "process.hpp"
#include <Tlhelp32.h> 
#include <ShlObj.h>
#pragma comment (lib, "shlwapi.lib")

namespace Util::Process {
	bool IsRunning(std::string processName) {
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

		bool exists = false;

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		BOOL loop = Process32First(snapshot, &entry);
		while (loop) {
			if (!strcmp(entry.szExeFile, processName.c_str())) {
				exists = true;
				break;
			}

			loop = Process32Next(snapshot, &entry);
		}

		CloseHandle(snapshot);
		return exists;
	}

	bool Create(std::string path) {
		PROCESS_INFORMATION processInformation;

		STARTUPINFO startupInfo;
		startupInfo.cb = sizeof(STARTUPINFO);

		wchar_t buffer[MAX_PATH];
		mbstowcs(buffer, path.c_str(), path.length() + 1);

		BOOL result = CreateProcess(NULL, (LPWSTR)buffer, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);
		if (result) {
			WaitForSingleObject(processInformation.hProcess, INFINITE);

			DWORD exitCode = 0;
			result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

			CloseHandle(processInformation.hProcess);
			CloseHandle(processInformation.hThread);

			if (result) return true;
		}

		return false;
	}
}