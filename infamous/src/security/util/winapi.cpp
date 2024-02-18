#include "stdafx.hpp"
#include "winapi.hpp"

namespace Security {
	namespace NtDll {
		// Define NtDll function cache.
		uint64_t g_functionCache[NUM_FUNCTIONS];
		// Define NtDll validity flag.
		bool g_bDllScrewed = false;

		NtOpenProcessFnPtr NtOpenProcess;
		NtQueryVirtualMemoryFnPtr NtQueryVirtualMemory;
		NtQuerySystemInformationFnPtr NtQuerySystemInformation;
		NtQueryInformationProcessFnPtr NtQueryInformationProcess;

		// Initializes the RageSec NtDll module.
		NtDllInitialiser g_NtDll;

		void InitNtDll()
		{
		
			HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
			NtOpenProcess = (NtOpenProcessFnPtr)GetProcAddress(hNtdll, "NtOpenProcess");
			NtQueryVirtualMemory = (NtQueryVirtualMemoryFnPtr)GetProcAddress(hNtdll, "NtQueryVirtualMemory");
			NtQuerySystemInformation = (NtQuerySystemInformationFnPtr)GetProcAddress(hNtdll, "NtQuerySystemInformation");
			NtQueryInformationProcess = (NtQueryInformationProcessFnPtr)GetProcAddress(hNtdll, "NtQueryInformationProcess");

			InitProtection();
		}
	}
}