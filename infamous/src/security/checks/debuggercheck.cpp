#include "stdafx.hpp"
#include "debuggercheck.hpp"
#include "../papi/types.hpp"
#include "../util/winapi.hpp"
#include "core/log/log.hpp"
#include "../security.hpp"
//wonder where i got this from
static unsigned int sm_ddTrashValue = 0;
enum Detections
{
	DD_NONE = 0xB54CE0B3,
	DD_ISDEBUGGER = 0xEB672B51,	// Needs testing/Support
	DD_ISDEBUGPEB = 0x40B85F02,	// Needs testing
	DD_TIMEDSTEPOVER = 0xE88A8F02,
	DD_NTQUERYINF = 0x7C4989AA,
	DD_NTGLOBALFLAGS = 0x8624A9A6,	// Needs testing
	DD_DEBUGREGISTERS = 0xEDA03F2F,	// Needs testing/Support
	DD_PROCESSDEBUGFLAGS = 0xDFD5974A,	// Implemented
	DD_PARENTPROCESS = 0x802F2597,
	DD_SYSTEMKERNELDEBUGGERINFORMATION = 0x942B6D4D,	// Implemented
	DD_DEBUGOBJECTFOUND = 0xC95A8A84,	// Implemented
	DD_VEHBREAKPOINT = 0xF07A7FC3,
	DD_IATHOOK = 0x2015F6E4,
	DD_CLOSEHANDLE = 0xC96CB241,	// Implemented
	DD_INT3 = 0x11493BE8,	// Implemented
	DD_INT2C = 0x0153572B,	// Implemented
	DD_DEBUGACTIVEPROCESS = 0x59E8C2FB,
	DD_HEAPFLAGS = 0x1D5D7268,	// Implemented
	DD_HEAPFORCEFLAGS = 0x5C21E644,	// Implemented
	DD_OUTPUTDEBUGSTRING = 0xC7999CC7,
	DD_FINDWINDOWCLASS = 0x979E36B5,
	DD_RAISE_RIP_EXCEPTION = 0xF5F55140,	// Implemented                                     
	DD_ALL = 0xADC0EF8D
};

namespace  Security::Checks::Debugger {

	bool BeingDebuggedPeb()
	{
		DWORD_PTR pPeb64 = __readgsqword(0x60);
		if (pPeb64)
		{
			PBYTE BeingDebugged = (PBYTE)pPeb64 + 0x2;
			if (*BeingDebugged)
			{
				return true;
			}
		}
		return false;
	}

	bool NtGlobalFlag()
	{
		DWORD_PTR pPeb64 = __readgsqword(0x60);
		if (pPeb64) {
			PDWORD pNtGlobalFlag = (PDWORD)((PBYTE)pPeb64 + 0xBC);
			if ((*pNtGlobalFlag & 0x70) == 0x70)
			{
				return true;
			}
		}
		return false;
	}

	bool DCheckDebugObjects()
	{

		HANDLE hDebugObject = 0;
		NTSTATUS status = (NtDll::NtQueryInformationProcess)(
			GetCurrentProcess(),
			0x1E,
			&hDebugObject,
			8,
			0);
		if (status == 0 && NULL != hDebugObject)
		{
			return true;
		}
		return false;
	}

	bool CheckProcessDebugFlags()
	{
		DWORD NoDebugInherit = 0;
		NTSTATUS status = (NtDll::NtQueryInformationProcess)(
			GetCurrentProcess(),
			0x1F,
			&NoDebugInherit,
			8,
			0);
		if (status == 0 && NoDebugInherit == FALSE)
		{
			return true;
		}
		return false;
	}

	bool CheckSystemKernelDebuggerInformation()
	{
		/*typedef long NTSTATUS;
		SYSTEM_KERNEL_DEBUGGER_INFORMATION Info;

		if (0 == (NtDll::NtQuerySystemInformation)(
			SystemKernelDebuggerInformation,
			&Info,
			sizeof(Info),
			NULL))
		{
			if (Info.DebuggerEnabled && !Info.DebuggerNotPresent)
			{
				return true;
			}
		}*/
		return false;
	}

	bool CheckINT3Handler()
	{
		__try
		{
			__debugbreak();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;;
		}

		return true;
	}

#pragma warning(disable : 4996)
	bool CheckINT2CHandler()
	{
		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);


		GetVersionEx(&osvi);

		if (osvi.dwMajorVersion < 6) //checks to see if we are on at least vista: NT 6.0
			return false;

		__try
		{
			__int2c();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}

		return true;
	}
#pragma warning(default: 4996)

	bool CheckHeapFlags()
	{
		DWORD_PTR pPeb64 = __readgsqword(0x60);
		PVOID heap = 0;
		PDWORD heapFlagsPtr = 0;

		if (pPeb64)
		{
			heap = (PVOID) * (PDWORD_PTR)((PBYTE)pPeb64 + 0x30);
			heapFlagsPtr = (PDWORD)((PBYTE)heap + 0x70);

			if (*heapFlagsPtr == 0x40000062) // Source: Ultimate Anti-Reversing Reference
			{
				return true;
			}
		}
		return false;
	}

	bool CheckHeapForceFlags()
	{
		DWORD_PTR pPeb64 = __readgsqword(0x60);
		PVOID heap = 0;
		PDWORD heapForceFlagsPtr = 0;

		if (pPeb64)
		{
			heap = (PVOID) * (PDWORD_PTR)((PBYTE)pPeb64 + 0x30);
			heapForceFlagsPtr = (PDWORD)((PBYTE)heap + 0x74);

			if (*heapForceFlagsPtr == 0x40000060) //Found abnormal heap flag
			{
				return true;
			}
		}
		return false;
	}

	bool CheckRaiseException()
	{
		__try
		{
			RaiseException(DBG_RIPEXCEPTION, 0, 0, 0);
		}
		__except (1)
		{
			return false;
		}
		return true;
	}

	bool CheckCloseHandle()
	{
		HANDLE Handle = (HANDLE)0x8000;
		__try
		{
			CloseHandle(Handle);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return true;
		}
		return false;
	}

	void Init() {
		if (BeingDebuggedPeb()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}

		if (NtGlobalFlag()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}

		if (DCheckDebugObjects()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}

		if (CheckProcessDebugFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}

		if (CheckHeapFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}

		if (CheckHeapForceFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
		}
	}

	bool Run() {
		if (BeingDebuggedPeb()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		if (NtGlobalFlag()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		if (DCheckDebugObjects()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		if (CheckProcessDebugFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		/*if (CheckSystemKernelDebuggerInformation()) {
			LOG_ERROR("Debugger Active");
		}*/

		if (CheckHeapFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		if (CheckHeapForceFlags()) {
			LOG_ERROR("Debugger Active");
			g_Running = false;
			return true;
		}

		return false;

	}
}