#pragma once
#include "../papi/types.hpp"

namespace Security {
	namespace NtDll
	{
		extern NtOpenProcessFnPtr NtOpenProcess;
		extern NtQueryVirtualMemoryFnPtr NtQueryVirtualMemory;
		extern NtQuerySystemInformationFnPtr NtQuerySystemInformation;
		extern NtQueryInformationProcessFnPtr NtQueryInformationProcess;

		extern void InitNtDll();

		// Identifiers for NtDll functions.
		enum NtFunctionsToVerify
		{
			NT_QUERY_VIRTUAL_MEMORY,
			NT_QUERY_SYSTEM_INFORMATION,
			NT_OPEN_PROCESS,
			NT_QUERY_INFORMATION_PROCSES,

			NUM_FUNCTIONS
		};
		// Global NtDll function cache that stores NtDll function addresses.
		extern uint64_t g_functionCache[NtFunctionsToVerify::NUM_FUNCTIONS];
		// Global flag to indicate if NtDll has been invalidated.
		extern bool g_bDllScrewed;

		// Store the first 8 bytes of tracked NtDll functions.
		inline void InitProtection()
		{
			g_functionCache[NT_OPEN_PROCESS] = *(uint64_t*)NtOpenProcess;
			g_functionCache[NT_QUERY_VIRTUAL_MEMORY] = *(uint64_t*)NtQueryVirtualMemory;
			g_functionCache[NT_QUERY_SYSTEM_INFORMATION] = *(uint64_t*)NtQuerySystemInformation;
			g_functionCache[NT_QUERY_INFORMATION_PROCSES] = *(uint64_t*)NtQueryInformationProcess;
		}

		// Verify that the first 8 bytes of each function have not changed.
		inline void VerifyProtection()
		{
			if (g_bDllScrewed == false &&
				(
					g_functionCache[NT_OPEN_PROCESS] != *(uint64_t*)NtOpenProcess ||
					g_functionCache[NT_QUERY_VIRTUAL_MEMORY] != *(uint64_t*)NtQueryVirtualMemory ||
					g_functionCache[NT_QUERY_SYSTEM_INFORMATION] != *(uint64_t*)NtQuerySystemInformation ||
					g_functionCache[NT_QUERY_INFORMATION_PROCSES] != *(uint64_t*)NtQueryInformationProcess
					)
				)
			{
				g_bDllScrewed = true;
			}
		}

		inline bool HasBeenHacked() { return g_bDllScrewed; }

		class NtDllInitialiser
		{
		public:
			NtDllInitialiser() { InitNtDll(); }
		};
	}
}